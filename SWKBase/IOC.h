#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <memory>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <utility>
#include <tuple>
#include <vector>
#include <cstdint>

namespace swktool {

    enum class object_type {
        Instance,
        Singleton,
    };

    // -----------------------------
    // Tag key for multi-mapping
    // -----------------------------
    struct TypeTagKey {
        std::type_index type;
        std::size_t tag; // 0 = default, otherwise enum value

        bool operator==(const TypeTagKey& other) const noexcept {
            return type == other.type && tag == other.tag;
        }
    };

    struct TypeTagKeyHash {
        std::size_t operator()(const TypeTagKey& k) const noexcept {
            std::size_t h1 = std::hash<std::type_index>()(k.type);
            std::size_t h2 = std::hash<std::size_t>()(k.tag);

            // 64-bit golden ratio constant truncated to size_t
            constexpr std::size_t magic = static_cast<std::size_t>(0x9e3779b97f4a7c15ULL);

            h2 += magic + (h1 << 6) + (h1 >> 2);
            return h1 ^ h2;
        }
    };



    // -----------------------------
    // Lazy<T> wrapper
    // -----------------------------
    template<typename T>
    class Lazy {
    public:
        Lazy() = default;

        explicit Lazy(std::function<std::shared_ptr<T>()> resolver)
            : resolver_(std::move(resolver)) {}

        std::shared_ptr<T> Get() const {
            if (!instance_) {
                if (!resolver_) {
                    throw std::runtime_error("Lazy: no resolver configured");
                }
                instance_ = resolver_();
            }
            return instance_;
        }

        bool IsValueCreated() const noexcept {
            return static_cast<bool>(instance_);
        }

    private:
        mutable std::shared_ptr<T> instance_;
        std::function<std::shared_ptr<T>()> resolver_;
    };

    // -----------------------------
    // Constructor traits (user specialization)
    // -----------------------------
    template<typename T>
    struct constructor_traits; // user must specialize: using args = std::tuple<...>;

    // helper: always_false
    template<typename T>
    struct always_false : std::false_type {};

    // is_shared_ptr trait
    template<typename T>
    struct is_shared_ptr : std::false_type {};

    template<typename U>
    struct is_shared_ptr<std::shared_ptr<U>> : std::true_type {};

    // -----------------------------
    // Setter traits for member functions
    // -----------------------------
    template<typename T>
    struct method_traits;

    template<typename C, typename R, typename... Args>
    struct method_traits<R(C::*)(Args...)> {
        using args = std::tuple<Args...>;
    };

    template<typename C, typename R, typename... Args>
    struct method_traits<R(C::*)(Args...) const> {
        using args = std::tuple<Args...>;
    };

    class IOCContainer {
    public:
        IOCContainer() = default;
        ~IOCContainer() = default;

        IOCContainer(const IOCContainer&) = delete;
        IOCContainer& operator=(const IOCContainer&) = delete;

        IOCContainer(IOCContainer&&) = default;
        IOCContainer& operator=(IOCContainer&&) = default;

        // -----------------------------
        // Core registration primitives
        // -----------------------------

        // Register I -> T with lifetime (default tag)
        template<typename I, typename T>
        void Register(object_type lifetime = object_type::Instance) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                return std::make_shared<T>();
            };

            registrations_.emplace(key, std::move(reg));
        }

        // Register I -> T with lifetime and enum tag
        template<typename I, typename T, typename TagEnum>
        void RegisterTagged(TagEnum tag, object_type lifetime = object_type::Instance) {
            static_assert(std::is_enum_v<TagEnum>, "TagEnum must be an enum type");
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            TypeTagKey key{ std::type_index(typeid(I)), static_cast<std::size_t>(tag) };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                return std::make_shared<T>();
            };

            registrations_.emplace(key, std::move(reg));
        }

        // Register existing object (Singleton semantics)
        template<typename I>
        void RegisterExisting(std::shared_ptr<I> instance) {
            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = object_type::Singleton;
            reg.factory = [instance](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                return instance;
            };
            reg.singletonInstance = instance;

            registrations_.emplace(key, std::move(reg));
        }

        template<typename I, typename TagEnum>
        void RegisterExistingTagged(TagEnum tag, std::shared_ptr<I> instance) {
            static_assert(std::is_enum_v<TagEnum>, "TagEnum must be an enum type");

            TypeTagKey key{ std::type_index(typeid(I)), static_cast<std::size_t>(tag) };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = object_type::Singleton;
            reg.factory = [instance](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                return instance;
            };
            reg.singletonInstance = instance;

            registrations_.emplace(key, std::move(reg));
        }

        // Register I -> T with a custom factory (no args, default tag)
        template<typename I, typename T, typename Factory>
        void RegisterFactory(object_type lifetime, Factory&& factory) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [f = std::forward<Factory>(factory)](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                std::shared_ptr<T> ptr = f();
                return std::static_pointer_cast<void>(ptr);
            };

            registrations_.emplace(key, std::move(reg));
        }

        // Register I -> T with a custom factory and tag
        template<typename I, typename T, typename TagEnum, typename Factory>
        void RegisterFactoryTagged(TagEnum tag, object_type lifetime, Factory&& factory) {
            static_assert(std::is_enum_v<TagEnum>, "TagEnum must be an enum type");
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            TypeTagKey key{ std::type_index(typeid(I)), static_cast<std::size_t>(tag) };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [f = std::forward<Factory>(factory)](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                std::shared_ptr<T> ptr = f();
                return std::static_pointer_cast<void>(ptr);
            };

            registrations_.emplace(key, std::move(reg));
        }

        // Replace / Override registration (default tag)
        template<typename I, typename T>
        void Replace(object_type lifetime = object_type::Instance) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                return std::make_shared<T>();
            };

            registrations_[key] = std::move(reg);
            InvalidateCache(key);
        }

        template<typename I, typename T, typename TagEnum>
        void ReplaceTagged(TagEnum tag, object_type lifetime = object_type::Instance) {
            static_assert(std::is_enum_v<TagEnum>, "TagEnum must be an enum type");
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            TypeTagKey key{ std::type_index(typeid(I)), static_cast<std::size_t>(tag) };

            std::lock_guard<std::mutex> lock(mutex_);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                return std::make_shared<T>();
            };

            registrations_[key] = std::move(reg);
            InvalidateCache(key);
        }

        // Replace with custom factory
        template<typename I, typename T, typename Factory>
        void ReplaceFactory(object_type lifetime, Factory&& factory) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [f = std::forward<Factory>(factory)](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                std::shared_ptr<T> ptr = f();
                return std::static_pointer_cast<void>(ptr);
            };

            registrations_[key] = std::move(reg);
            InvalidateCache(key);
        }

        // -----------------------------
        // Auto-wiring: constructor-based
        // -----------------------------
        // T must have constructor_traits<T>::args defined as std::tuple<Deps...>
        template<typename I, typename T>
        void RegisterAutoCtor(object_type lifetime = object_type::Instance) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            using args_tuple = typename constructor_traits<T>::args;

            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [this](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                return std::static_pointer_cast<void>(
                    ConstructFromArgsTuple<T>(typename constructor_traits<T>::args{})
                );
            };

            registrations_.emplace(key, std::move(reg));
        }

        // -----------------------------
        // Auto-wiring: setter-based
        // -----------------------------
        // T must be default-constructible; Setter is pointer to member function
        template<typename I, typename T, typename Setter>
        void RegisterAutoSetter(Setter setter, object_type lifetime = object_type::Instance) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            using args_tuple = typename method_traits<Setter>::args;

            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);
            EnsureNotRegistered(key);

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [this, setter](void* /*argsTuplePtr*/) -> std::shared_ptr<void> {
                auto obj = std::make_shared<T>();
                CallSetterWithArgsTuple(obj.get(), setter, typename method_traits<Setter>::args{});
                return std::static_pointer_cast<void>(obj);
            };

            registrations_.emplace(key, std::move(reg));
        }

        // -----------------------------
        // Resolve APIs
        // -----------------------------

        // Resolve as shared_ptr<I> (default tag)
        template<typename I>
        std::shared_ptr<I> ResolveShared() {
            TypeTagKey key{ std::type_index(typeid(I)), 0 };
            return ResolveSharedKey<I>(key);
        }

        // Resolve as shared_ptr<I> with tag
        template<typename I, typename TagEnum>
        std::shared_ptr<I> ResolveSharedTagged(TagEnum tag) {
            static_assert(std::is_enum_v<TagEnum>, "TagEnum must be an enum type");
            TypeTagKey key{ std::type_index(typeid(I)), static_cast<std::size_t>(tag) };
            return ResolveSharedKey<I>(key);
        }

        // Resolve as unique_ptr<I> (only valid for Instance lifetime, default tag)
        template<typename I>
        std::unique_ptr<I> ResolveUnique() {
            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);
            auto it = registrations_.find(key);
            if (it == registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface not registered");
            }

            Registration& reg = it->second;

            if (reg.lifetime == object_type::Singleton) {
                throw std::runtime_error("IOCContainer: cannot resolve singleton as unique_ptr");
            }

            auto obj = ResolveInternal(key, reg, nullptr);
            I* raw = static_cast<I*>(obj.get());
            obj.reset();
            return std::unique_ptr<I>(raw);
        }

        // Resolve as raw pointer (shared lifetime)
        template<typename I>
        I* ResolveRaw() {
            return ResolveShared<I>().get();
        }

        // Resolve with constructor arguments (perfect forwarding, default tag)
        template<typename I, typename... Args>
        std::shared_ptr<I> ResolveWithArgs(Args&&... args) {
            TypeTagKey key{ std::type_index(typeid(I)), 0 };

            std::lock_guard<std::mutex> lock(mutex_);
            auto it = registrations_.find(key);
            if (it == registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface not registered");
            }

            Registration& reg = it->second;

            auto tuple = std::make_tuple(std::forward<Args>(args)...);
            auto obj = ResolveInternal(key, reg, static_cast<void*>(&tuple), /*useCache*/false);
            return std::static_pointer_cast<I>(obj);
        }

        // TryResolve variant that returns nullptr instead of throwing
        template<typename I>
        std::shared_ptr<I> TryResolveShared() noexcept {
            try {
                return ResolveShared<I>();
            }
            catch (...) {
                return nullptr;
            }
        }

        // Lazy resolution
        template<typename I>
        Lazy<I> MakeLazy() {
            return Lazy<I>([this]() {
                return this->ResolveShared<I>();
            });
        }

        template<typename I, typename TagEnum>
        Lazy<I> MakeLazyTagged(TagEnum tag) {
            static_assert(std::is_enum_v<TagEnum>, "TagEnum must be an enum type");
            return Lazy<I>([this, tag]() {
                return this->ResolveSharedTagged<I>(tag);
            });
        }

        bool IsRegistered(std::type_index type, std::size_t tag = 0) const {
            std::lock_guard<std::mutex> lock(mutex_);
            TypeTagKey key{ type, tag };
            return registrations_.find(key) != registrations_.end();
        }

        template<typename I>
        bool IsRegistered() const {
            return IsRegistered(std::type_index(typeid(I)), 0);
        }

        template<typename I, typename TagEnum>
        bool IsRegisteredTagged(TagEnum tag) const {
            static_assert(std::is_enum_v<TagEnum>, "TagEnum must be an enum type");
            return IsRegistered(std::type_index(typeid(I)), static_cast<std::size_t>(tag));
        }

    private:
        struct Registration {
            object_type lifetime{};
            std::function<std::shared_ptr<void>(void*)> factory;
            std::shared_ptr<void> singletonInstance;

            // adaptive resolve caching
            std::uint64_t resolveCount{ 0 };
            std::shared_ptr<void> hotCache;
        };

        static constexpr std::uint64_t HOT_THRESHOLD = 50;

        mutable std::mutex mutex_;
        std::unordered_map<TypeTagKey, Registration, TypeTagKeyHash> registrations_;

        // Circular dependency detection: thread-local stack
        static inline thread_local std::vector<TypeTagKey> resolvingStack_;

        void EnsureNotRegistered(const TypeTagKey& key) {
            if (registrations_.find(key) != registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface already registered");
            }
        }

        void InvalidateCache(const TypeTagKey& key) {
            auto it = registrations_.find(key);
            if (it != registrations_.end()) {
                it->second.resolveCount = 0;
                it->second.hotCache.reset();
                if (it->second.lifetime == object_type::Singleton) {
                    it->second.singletonInstance.reset();
                }
            }
        }

        template<typename I>
        std::shared_ptr<I> ResolveSharedKey(const TypeTagKey& key) {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = registrations_.find(key);
            if (it == registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface not registered");
            }

            Registration& reg = it->second;
            auto obj = ResolveInternal(key, reg, nullptr);
            return std::static_pointer_cast<I>(obj);
        }

        std::shared_ptr<void> ResolveInternal(const TypeTagKey& key, Registration& reg, void* argsTuplePtr, bool useCache = true) {
            // circular detection
            for (const auto& k : resolvingStack_) {
                if (k.type == key.type && k.tag == key.tag) {
                    throw std::runtime_error("IOCContainer: circular dependency detected");
                }
            }

            resolvingStack_.push_back(key);

            std::shared_ptr<void> result;

            if (reg.lifetime == object_type::Singleton) {
                if (!reg.singletonInstance) {
                    reg.singletonInstance = reg.factory(argsTuplePtr);
                }
                result = reg.singletonInstance;
            }
            else {
                if (useCache) {
                    reg.resolveCount++;
                    if (reg.resolveCount > HOT_THRESHOLD) {
                        if (!reg.hotCache) {
                            reg.hotCache = reg.factory(argsTuplePtr);
                        }
                        result = reg.hotCache;
                    }
                    else {
                        result = reg.factory(argsTuplePtr);
                    }
                }
                else {
                    result = reg.factory(argsTuplePtr);
                }
            }

            resolvingStack_.pop_back();
            return result;
        }

        template<typename I>
        std::shared_ptr<I> ResolveSharedInternal() {
            TypeTagKey key{ std::type_index(typeid(I)), 0 };
            auto it = registrations_.find(key);
            if (it == registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface not registered (internal auto-wiring)");
            }
            Registration& reg = it->second;
            auto obj = ResolveInternal(key, reg, nullptr);
            return std::static_pointer_cast<I>(obj);
        }

        // -----------------------------
        // Auto-ctor helpers
        // -----------------------------
        template<typename Dep>
        auto ResolveArg() {
            if constexpr (is_shared_ptr<Dep>::value) {
                using Elem = typename Dep::element_type;
                return ResolveShared<Elem>();
            }
            else {
                static_assert(always_false<Dep>::value, "Unsupported dependency type in constructor_traits<T>::args. Use std::shared_ptr<Dep>.");
            }
        }

        template<typename T, typename Tuple, std::size_t... Is>
        std::shared_ptr<T> ConstructFromArgsTupleImpl(Tuple, std::index_sequence<Is...>) {
            return std::make_shared<T>(
                ResolveArg<std::tuple_element_t<Is, Tuple>>()...
            );
        }

        template<typename T, typename Tuple>
        std::shared_ptr<T> ConstructFromArgsTuple(Tuple t) {
            constexpr std::size_t N = std::tuple_size_v<Tuple>;
            return ConstructFromArgsTupleImpl<T>(t, std::make_index_sequence<N>{});
        }

        // -----------------------------
        // Auto-setter helpers
        // -----------------------------
        template<typename Dep>
        auto ResolveSetterArg() {
            if constexpr (is_shared_ptr<Dep>::value) {
                using Elem = typename Dep::element_type;
                return ResolveShared<Elem>();
            }
            else if constexpr (std::is_pointer_v<Dep>) {
                using Elem = std::remove_pointer_t<Dep>;
                return ResolveShared<Elem>().get();
            }
            else {
                static_assert(always_false<Dep>::value, "Unsupported dependency type in setter args. Use std::shared_ptr<Dep> or Dep*.");
            }
        }

        template<typename Obj, typename Setter, typename Tuple, std::size_t... Is>
        void CallSetterWithArgsTupleImpl(Obj* obj, Setter setter, Tuple, std::index_sequence<Is...>) {
            (obj->*setter)(ResolveSetterArg<std::tuple_element_t<Is, Tuple>>()...);
        }

        template<typename Obj, typename Setter, typename Tuple>
        void CallSetterWithArgsTuple(Obj* obj, Setter setter, Tuple t) {
            constexpr std::size_t N = std::tuple_size_v<Tuple>;
            CallSetterWithArgsTupleImpl(obj, setter, t, std::make_index_sequence<N>{});
        }
    };

    //thread_local std::vector<TypeTagKey> IOCContainer::resolvingStack_;

} // namespace swktool
