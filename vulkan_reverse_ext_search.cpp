#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vulkan/vulkan_extension_inspection.hpp>
#include <algorithm>
#include <string_view>
#include <format>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " vulkan extensions to reverse dep find" << std::endl;
        return 1;
    }

    std::cout << std::format("Known vulkan version: {}.{}.{}\n",
        vk::apiVersionMajor(vk::HeaderVersionComplete),
        vk::apiVersionMinor(vk::HeaderVersionComplete),
        vk::apiVersionPatch(vk::HeaderVersionComplete)); 
    
    std::vector<std::string> to_find;
    for (int i = 1; i < argc; ++i) {
        auto el = to_find.emplace_back(argv[i]);
        if (!vk::isDeviceExtension(el) && !vk::isInstanceExtension(el)) {
            std::cout << el << " is not a valid vulkan extension" << std::endl;
            return 1;
        }
    }

    for (auto&ext : vk::getDeviceExtensions()) {
        auto [has_dep, depss] = vk::getExtensionDepends("VK_VERSION_1_3", ext);
        for (auto&deps : depss) {
            for (auto&dep : deps)
                if (std::find(std::begin(to_find), std::end(to_find), dep) != std::end(to_find)) {
                    std::cout << ext << " depends on " << dep << std::endl;
                }
        }
    }
}
