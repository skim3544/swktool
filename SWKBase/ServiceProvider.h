#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <any>



class IServiceProvider 
{
public:
    virtual ~IServiceProvider() = default;

    // Register an already-created instance (singleton)
    template<typename T>
    void RegisterInstance(T* instance);

    // Register a factory that creates T on demand (transient)
    template<typename T, typename Factory>
    void RegisterFactory(Factory&& factory);

    // Resolve a service by type
    template<typename T>
    T* Resolve();
};



class ServiceProvider : public IServiceProvider
{
public:
    ServiceProvider() = default;
    ~ServiceProvider() = default;

    // Register an existing instance (singleton)
    template<typename T>
    void RegisterInstance(T* instance)
    {
        factories_[std::type_index(typeid(T))] =
            std::function<T * ()>([instance]() {
            return instance;
                });
    }

    // Register a factory (transient or custom creation)
    template<typename T, typename Factory>
    void RegisterFactory(Factory&& factory)
    {
        // Store a type-safe factory for T
        factories_[std::type_index(typeid(T))] =
            std::function<T * (auto&&...)>(
                [f = std::forward<Factory>(factory)](auto&&... args) -> T* {
                    return f(std::forward<decltype(args)>(args)...);
                }
            );
    }

    // Resolve a service with perfect forwarding
    template<typename T, typename... Args>
    T* Resolve(Args&&... args)
    {
        auto it = factories_.find(std::type_index(typeid(T)));
        if (it == factories_.end())
            return nullptr;

        // Extract the stored factory
        auto& fn = std::any_cast<std::function<T * (Args&&...)>&>(it->second);
        return fn(std::forward<Args>(args)...);
    }

private:
    // Each entry stores a type-safe std::function<T*(Args...)>
    std::unordered_map<std::type_index, std::any> factories_;
};
