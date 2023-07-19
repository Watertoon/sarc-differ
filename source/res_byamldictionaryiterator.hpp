#pragma once

namespace vp::res {

    namespace {

        ALWAYS_INLINE u32 RemapIndex(void *remap_table, u32 data_count, u32 index, u32 is_reverse_endian) {
            if (data_count < 0x100)   { return *reinterpret_cast<u8*>(reinterpret_cast<uintptr_t>(remap_table) + index * sizeof(u8)); }
            if (data_count < 0x10000) { 
                u16 *remap_data = reinterpret_cast<u16*>(reinterpret_cast<uintptr_t>(remap_table) + index * sizeof(u16));
                return (is_reverse_endian == false) ? *remap_data : vp::util::SwapEndian(*remap_data);
            }
            u32 *remap_data = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(remap_table) + index * sizeof(u32));
            return (is_reverse_endian == false) ? *remap_data : vp::util::SwapEndian(*remap_data);
        }
    }

    class ByamlDictionaryIterator {
        private:
            const ResByamlContainer *m_byaml_container;
        public:
            constexpr ALWAYS_INLINE ByamlDictionaryIterator() : m_byaml_container(nullptr) {/*...*/}
            constexpr ALWAYS_INLINE ByamlDictionaryIterator(const ResByamlContainer *container) : m_byaml_container(container) {/*...*/}

            constexpr ALWAYS_INLINE ~ByamlDictionaryIterator() {/*...*/}

            ALWAYS_INLINE bool TryGetDataByIndex(ByamlData *out_data, u32 index, u32 is_reverse_endian) const {

                /* Integrity checks */
                if (m_byaml_container == nullptr) { return false; }

                /* Get data couunt */
                const u32 data_count = this->GetCount(is_reverse_endian);

                /* Bounds check index */
                if (data_count <= index) { return false; }

                /* Remap index if necessary */
                if (static_cast<ByamlDataType>(m_byaml_container->data_type) == ByamlDataType::DictionaryWithRemap) {
                    const u32 remap_table_offset = data_count * sizeof(ResByamlDictionaryPair) + sizeof(u32);
                    index = RemapIndex(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_byaml_container) + remap_table_offset), data_count, index, is_reverse_endian);
                }

                /* Get dictionary pair by index */
                const ResByamlDictionaryPair *pair = reinterpret_cast<const ResByamlDictionaryPair*>(reinterpret_cast<uintptr_t>(m_byaml_container) + sizeof(ResByamlContainer) + sizeof(ResByamlDictionaryPair) * index);
                out_data->SetPair(pair, is_reverse_endian);

                return true;
            }

            ALWAYS_INLINE const ResByamlDictionaryPair *GetDictionaryPairByIndex(u32 index, u32 is_reverse_endian) const {

                /* Integrity checks */
                if (m_byaml_container == nullptr || this->GetCount(is_reverse_endian) <= index) { return nullptr; }

                /* Get dictionary pair */
                return reinterpret_cast<const ResByamlDictionaryPair*>(reinterpret_cast<uintptr_t>(m_byaml_container) + sizeof(ResByamlContainer) + sizeof(ResByamlDictionaryPair) * index);
            }

            ALWAYS_INLINE u32 TryRemapIndex(u32 index, u32 is_reverse_endian) const {

                /* Integrity checks */
                if (m_byaml_container == nullptr) { return false; }

                /* Get data couunt */
                const u32 data_count = this->GetCount(is_reverse_endian);

                /* Bounds check index */
                if (data_count <= index) { return false; }

                /* Remap index if necessary */
                if (static_cast<ByamlDataType>(m_byaml_container->data_type) == ByamlDataType::DictionaryWithRemap) {
                    const u32 remap_table_offset = data_count * sizeof(ResByamlDictionaryPair) + sizeof(u32);
                    index = RemapIndex(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_byaml_container) + remap_table_offset), data_count, index, is_reverse_endian);
                }

                return index;
            }

            constexpr ALWAYS_INLINE u32 GetCount(u32 is_reverse_endian) const {
                return (is_reverse_endian == false) ? static_cast<u32>(m_byaml_container->count) : vp::util::SwapEndian24(static_cast<u32>(m_byaml_container->count));
            }
    };

    class ByamlArrayIterator {
        private:
            const ResByamlContainer *m_byaml_container;
        public:
            constexpr ALWAYS_INLINE ByamlArrayIterator() : m_byaml_container(nullptr) {/*...*/}
            constexpr ALWAYS_INLINE ByamlArrayIterator(const ResByamlContainer *container) : m_byaml_container(container) {/*...*/}

            constexpr ALWAYS_INLINE ~ByamlArrayIterator() {/*...*/}

            ALWAYS_INLINE bool TryGetDataByIndex(ByamlData *out_data, u32 index, u32 is_reverse_endian) const {

                /* Integrity checks */
                const u32 data_count = this->GetCount(is_reverse_endian);
                if (m_byaml_container == nullptr || data_count <= index) { return false; }

                /* Select data type index */
                u32 data_type_index    = index;
                u32 value_array_offset = dd::util::AlignUp(data_count, 4);
                if (static_cast<ByamlDataType>(m_byaml_container->data_type) == ByamlDataType::MonoTypedArray) {
                    data_type_index    = 0;
                    value_array_offset = sizeof(u32);
                }

                /* Calculate offsets */
                const u32 data_type_offset = sizeof(ResByamlContainer) + data_type_index;
                const u32 value_offset     = sizeof(ResByamlContainer) + value_array_offset + sizeof(u32) * index;

                /* Apply offsets */
                const u8   type       = *reinterpret_cast<u8*>(reinterpret_cast<uintptr_t>(m_byaml_container) + data_type_offset);
                const u32 *value_data = reinterpret_cast<const u32*>(reinterpret_cast<uintptr_t>(m_byaml_container) + value_offset);
                const u32  value      = (is_reverse_endian == false) ? *value_data : vp::util::SwapEndian(*value_data);                

                /* Set data */
                out_data->key_index = ByamlData::cInvalidKeyIndex;
                out_data->data_type = type;
                out_data->u32_value = value;

                return true;
            }

            constexpr ALWAYS_INLINE u32 GetCount(u32 is_reverse_endian) const {
                return (is_reverse_endian == false) ? static_cast<u32>(m_byaml_container->count) : vp::util::SwapEndian24(static_cast<u32>(m_byaml_container->count));
            }
    };

    class ByamlHashArrayIterator {
        private:
            const ResByamlContainer *m_byaml_container;
            u32                      m_stride;
        public:
            constexpr ALWAYS_INLINE ByamlHashArrayIterator(const ResByamlContainer *container) : m_byaml_container(container), m_stride((container->data_type & 0xf) * sizeof(u32) + sizeof(ResByamlContainer)) {/*...*/}
            constexpr ALWAYS_INLINE ByamlHashArrayIterator(const ResByamlContainer *container, u32 stride) : m_byaml_container(container), m_stride(stride) {/*...*/}

            constexpr ALWAYS_INLINE ~ByamlHashArrayIterator() {/*...*/}

            ALWAYS_INLINE bool TryGetDataByIndex(ByamlData *out_data, u32 index, u32 is_reverse_endian) const {

                /* Integrity check */
                if (m_byaml_container == nullptr) { return false; }

                /* Get array size */
                const u32 data_count = this->GetCount(is_reverse_endian);

                /* Bounds check index */
                if (data_count <= index) { return false; }

                /* Remap index if necessary */
                if (static_cast<ByamlDataType>(m_byaml_container->data_type & 0xf0) == ByamlDataType::HashArrayWithRemapU32_1) {
                    const u32 remap_table_offset = dd::util::AlignUp((m_stride + sizeof(ResByamlContainer) + sizeof(u8)) * data_count, sizeof(u32));
                    index = RemapIndex(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_byaml_container) + remap_table_offset), data_count, index, is_reverse_endian);
                }

                /* Calculate data offsets */
                const u32 data_type_offset = (m_stride + sizeof(u32)) * data_count + index * sizeof(u8);
                const u32 value_offset     = (m_stride + sizeof(u32)) * index + m_stride + sizeof(ResByamlContainer);

                /* Set data */
                out_data->key_index   = index;
                out_data->data_type   = *reinterpret_cast<u8*>(reinterpret_cast<uintptr_t>(m_byaml_container) + data_type_offset);
                const u32 *value_data = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(m_byaml_container) + value_offset);
                out_data->u32_value   = (is_reverse_endian == false) ? *value_data : vp::util::SwapEndian(*value_data);

                return true;
            }

            ALWAYS_INLINE u8 GetDataType(u32 index, u32 is_reverse_endian) const {
                const u32 data_count = this->GetCount(is_reverse_endian);
                const u32 data_type_offset = (m_stride + sizeof(u32)) * data_count + index * sizeof(u8);
                return *reinterpret_cast<u8*>(reinterpret_cast<uintptr_t>(m_byaml_container) + data_type_offset);
            }

            constexpr ALWAYS_INLINE u32 GetCount(u32 is_reverse_endian) const {
                return (is_reverse_endian == false) ? static_cast<u32>(m_byaml_container->count) : vp::util::SwapEndian24(static_cast<u32>(m_byaml_container->count));
            }
    };

    class ByamlHashAccessor {
        private:
            const u32 *m_hash_array;
            u32        m_stride;
        public:
            constexpr ByamlHashAccessor(const ResByamlContainer *container, u32 stride, u32 index) : m_hash_array(reinterpret_cast<const u32*>(reinterpret_cast<uintptr_t>(container) + (stride + sizeof(u32)) * index + sizeof(u32))), m_stride(stride) {/*...*/}
            constexpr ~ByamlHashAccessor() {/*...*/}

            ALWAYS_INLINE u64 GetHash(u32 is_reverse_endian) const {
                if (m_stride == sizeof(u64)) { return (is_reverse_endian == false) ? *reinterpret_cast<const u64*>(m_hash_array) : vp::util::SwapEndian(*reinterpret_cast<const u64*>(m_hash_array)); }
                if (m_stride == sizeof(u32)) { return (is_reverse_endian == false) ? *m_hash_array : vp::util::SwapEndian(*m_hash_array); }
                return 0;
            }
            ALWAYS_INLINE u32 GetValue(u32 is_reverse_endian) const {
                const u32 *value_data = reinterpret_cast<const u32*>(reinterpret_cast<uintptr_t>(m_hash_array) + m_stride);
                return (is_reverse_endian == false) ? *value_data : vp::util::SwapEndian(*value_data);
            }
    };
}
