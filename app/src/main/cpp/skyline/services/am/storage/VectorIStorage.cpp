// SPDX-License-Identifier: MPL-2.0
// Copyright © 2022 Skyline Team and Contributors (https://github.com/skyline-emu/)

#include "VectorIStorage.h"

#include <utility>

namespace skyline::service::am {
    /**
     * @brief IStorage is used to open an IStorageAccessor to access a region of memory
     * @url https://switchbrew.org/wiki/Applet_Manager_services#IStorage
     */
    VectorIStorage::VectorIStorage(const DeviceState &state, ServiceManager &manager, size_t size) : content(size, 0), IStorage(state, manager, true) {}

    VectorIStorage::VectorIStorage(const DeviceState &state, ServiceManager &manager, std::vector<u8> data) : content(std::move(data)), IStorage(state, manager, true) {}

    span<u8> VectorIStorage::GetSpan() {
        return {content};
    }

    VectorIStorage::~VectorIStorage() = default;
}