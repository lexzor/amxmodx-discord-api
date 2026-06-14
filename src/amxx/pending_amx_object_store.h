#pragma once

#include <unordered_map>
#include <functional>
#include <algorithm>

using PendingAmxObjectStoreHandle = uint32_t;

class PendingAmxObjectStore
{
public:
    using PendingAmxObjDeleterFn = std::function<void(uintptr_t)>;
    struct StoreObject
    {
        uintptr_t obj_ptr;
        PendingAmxObjDeleterFn deleter_fn;
    };

    using ForEachFn = std::function<void(PendingAmxObjectStoreHandle, StoreObject)>;

    PendingAmxObjectStore() {};
    ~PendingAmxObjectStore()
    {
        for (const auto& [handle, store_object] : m_ObjectStore)
        {
            store_object.deleter_fn(store_object.obj_ptr);
        }
    };

    template <typename ObjectType, typename... Args>
    PendingAmxObjectStoreHandle CreateObject(Args&&... args)
    {
        PendingAmxObjectStoreHandle handle;

        if (m_ObjectAvailableHandles.size())
        {
            handle = m_ObjectAvailableHandles.back();
            m_ObjectAvailableHandles.pop_back();
        }
        else handle = ++m_ObjectCount;

        StoreObject storeObj = {};
        storeObj.obj_ptr = reinterpret_cast<uintptr_t>(new ObjectType(std::forward<Args>(args)...));
        storeObj.deleter_fn = [](uintptr_t obj_ptr) { delete reinterpret_cast<ObjectType*>(obj_ptr); };

        m_ObjectStore[handle] = storeObj;

        return handle;
    }

    bool RemoveObject(PendingAmxObjectStoreHandle handle)
    {
        const ObjectStoreMap::iterator it = m_ObjectStore.find(handle);
    
        if (it == m_ObjectStore.end()) [[unlikely]]
            return false;

        m_ObjectAvailableHandles.push_back(it->first);
        m_ObjectStore.erase(it->first);

        return true;
    }

    template <typename ObjectType>
    ObjectType* GetStoreObject(PendingAmxObjectStoreHandle handle)
    {
        const ObjectStoreMap::iterator it = m_ObjectStore.find(handle);

        if (it == m_ObjectStore.end())
            return nullptr;

        return reinterpret_cast<ObjectType*>(it->second.obj_ptr);
    }

    uint32_t Clear()
    {
        for (const auto& [handle, store_object] : m_ObjectStore)
        {
            store_object.deleter_fn(store_object.obj_ptr);
        }

        uint32_t deletedObjectsCount = m_ObjectStore.size();

        m_ObjectStore.clear();
        m_ObjectAvailableHandles.clear();
        m_ObjectCount = 0;

        return deletedObjectsCount;
    }

    void ForEach(ForEachFn function)
    {
        for (auto& [handle, store_object] : m_ObjectStore)
        {
            function(handle, store_object);
        }
    }

private:
    using ObjectStoreMap = std::unordered_map<PendingAmxObjectStoreHandle, StoreObject>;

    ObjectStoreMap m_ObjectStore = {};
    std::vector<PendingAmxObjectStoreHandle> m_ObjectAvailableHandles = {};

    uint32_t m_ObjectCount = 0;
};