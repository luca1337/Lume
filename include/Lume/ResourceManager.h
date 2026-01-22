#pragma once

#include <Lume/Enums.h>
#include <Lume/Logger.h>
#include <memory>
#include <optional>
#include <ranges>

class IResource;

template <std::derived_from<IResource> Resource>
struct ResourceParams
{
    ResourceType mResourceType = {};
    std::string mName = {};
    std::shared_ptr<Resource> mResource = {};
};

template <std::derived_from<IResource> Resource>
using Resources = std::vector<ResourceParams<Resource>>;

class ResourceManager
{
public:
    template <std::derived_from<IResource> Resource>
    static void register_resource(const ResourceParams<Resource>& params)
    {
        if (!find_private(params)) mResources<Resource>.push_back(params);
        else LOG_CRITICAL("This Resource will not be loaded as it's already registered, please use GetFromCache() to retrieve it.");
    }

    template <std::derived_from<IResource> Resource>
    [[nodiscard]] static auto get_from_cache(const ResourceParams<Resource>& params) -> std::optional<decltype(std::declval<ResourceParams<Resource>>().mResource)> { return find_private(params).value().mResource; }

    template <std::derived_from<IResource> Resource>
    static auto get_all_from_cache() { return mResources<Resource>; }

private:
    template <std::derived_from<IResource> Resource>
    inline static Resources<Resource> mResources = {};

    template <std::derived_from<IResource> Resource, typename ReturnType = typename std::optional<ResourceParams<Resource>>::value_type>
    static auto find_private(const ResourceParams<Resource>& params) -> std::optional<ReturnType>
    {
        auto it = std::ranges::find(mResources<Resource>, params.mName, &ResourceParams<Resource>::mName);
        return it != mResources<Resource>.end() ? *it : std::optional<ReturnType>{};
    }
};