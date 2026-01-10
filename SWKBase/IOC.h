#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <memory>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <utility>

namespace swktool {

    enum class object_type {
        Instance,
        Singleton,
    };

    class IOCContainer {
    public:
        IOCContainer() = default;
        ~IOCContainer() = default;

        IOCContainer(const IOCContainer&) = delete;
        IOCContainer& operator=(const IOCContainer&) = delete;

        IOCContainer(IOCContainer&&) = default;
        IOCContainer& operator=(IOCContainer&&) = default;

        // Register I -> T with lifetime
        template<typename I, typename T>
        void Register(object_type lifetime = object_type::Instance) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            const std::type_index key(typeid(I));

            std::lock_guard<std::mutex> lock(mutex_);

            // If already registered, you can either:
            //  - throw, or
            //  - allow overwrite. Here, we throw to avoid silent surprises.
            auto it = registrations_.find(key);
            if (it != registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface already registered");
            }

            Registration reg;
            reg.lifetime = lifetime;
            reg.factory = [](void* argsTuplePtr) -> std::shared_ptr<void> {
                // Default factory without args: T must be default-constructible
                return std::make_shared<T>();
                };

            registrations_.emplace(key, std::move(reg));
        }

        // Register I -> T with a custom factory (for constructor args, etc.)
        template<typename I, typename T, typename Factory>
        void RegisterFactory(object_type lifetime, Factory&& factory) {
            static_assert(std::is_base_of_v<I, T> || std::is_same_v<I, T>,
                "T must derive from I or be the same type");

            const std::type_index key(typeid(I));

            std::lock_guard<std::mutex> lock(mutex_);

            auto it = registrations_.find(key);
            if (it != registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface already registered");
            }

            Registration reg;
            reg.lifetime = lifetime;

            // Wrap the factory into a type-erased std::function
            // Factory is expected to return std::shared_ptr<T> and take arbitrary args.
            reg.factory = [f = std::forward<Factory>(factory)](void* argsTuplePtr) -> std::shared_ptr<void> {
                // argsTuplePtr is optional; if you need args, use ResolveWithFactoryArgs below.
                (void)argsTuplePtr;
                std::shared_ptr<T> ptr = f();
                return std::static_pointer_cast<void>(ptr);
                };

            registrations_.emplace(key, std::move(reg));
        }

        // Resolve as shared_ptr<I>
        template<typename I>
        std::shared_ptr<I> ResolveShared() {
            const std::type_index key(typeid(I));

            std::lock_guard<std::mutex> lock(mutex_);

            auto it = registrations_.find(key);
            if (it == registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface not registered");
            }

            Registration& reg = it->second;

            if (reg.lifetime == object_type::Singleton) {
                if (!reg.singletonInstance) {
                    reg.singletonInstance = reg.factory(nullptr);
                }
                return std::static_pointer_cast<I>(reg.singletonInstance);
            }

            // Instance
            std::shared_ptr<void> obj = reg.factory(nullptr);
            return std::static_pointer_cast<I>(obj);
        }

        // Resolve as unique_ptr<I> (only valid for Instance lifetime)
        template<typename I>
        std::unique_ptr<I> ResolveUnique() {
            const std::type_index key(typeid(I));

            std::lock_guard<std::mutex> lock(mutex_);

            auto it = registrations_.find(key);
            if (it == registrations_.end()) {
                throw std::runtime_error("IOCContainer: interface not registered");
            }

            Registration& reg = it->second;

            if (reg.lifetime == object_type::Singleton) {
                throw std::runtime_error("IOCContainer: cannot resolve singleton as unique_ptr");
            }

            std::shared_ptr<void> obj = reg.factory(nullptr);
            I* raw = static_cast<I*>(obj.get());

            // Transfer ownership into unique_ptr by forgetting about the shared_ptr.
            // This assumes the factory creates a new object each time and does not
            // share ownership elsewhere.
            obj.reset();

            return std::unique_ptr<I>(raw);
        }

        // Resolve as raw pointer (shared lifetime unless Instance + custom usage)
        template<typename I>
        I* ResolveRaw() {
            return ResolveShared<I>().get();
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

        bool IsRegistered(std::type_index key) const {
            std::lock_guard<std::mutex> lock(mutex_);
            return registrations_.find(key) != registrations_.end();
        }

        template<typename I>
        bool IsRegistered() const {
            return IsRegistered(std::type_index(typeid(I)));
        }

    private:
        struct Registration {
            object_type lifetime{};
            std::function<std::shared_ptr<void>(void*)> factory;
            std::shared_ptr<void> singletonInstance;
        };

        mutable std::mutex mutex_;
        std::unordered_map<std::type_index, Registration> registrations_;
    };

} // namespace swktool
