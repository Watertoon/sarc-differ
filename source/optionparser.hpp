 /*
 *  Copyright (C) W. Michael Knudson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with this program; 
 *  if not, see <https://www.gnu.org/licenses/>.
 */
#pramga once

struct OptionEntry {
    const char *option_name;
    const char *option_value;
    u32         argv_index;
};

template <size_t Size>
class OptionParser {
    private:
        util::FixedObjectAllocator<OptionEntry, Size>   m_option_allocator;
        util::FixedPointerArray<OptionEntry, Size>      m_allocated_options;
        const char                                    **m_extra_args;
        u32                                             m_extra_arg_count;
    public:
        constexpr OptionParser() {/*...*/}

        void AddOption(const char *option_name) {

            /* Allocate new entry */
            OptionEntry *new_entry = m_option_allocator.Allocate();
            if (new_entry == nullptr) { return; }

            /* Set options */
            new_entry->option_name = option_name;

            /* Add to allocated array */
            m_allocated_options.PushPointer(new_entry);
        }

        void Parse(int argc, char **argv) {

            /* Find and use valid options */
            for (u32 i = 0; i < m_allocated_options.GetUsedCount(); ++i) {
                for (int j = 0; j < argc - 1; ++j) {
                    if (::strcmp(m_allocated_options[i]->option_name, argv[j]) == 0) {
                        m_allocated_options[i]->option_value = argv[j + 1];
                        m_allocated_options[i]->argv_index   = j;
                        ++j;
                    }
                }
            }
        }

        u32 FindArgvIndex(const char *option_name) {

            /* Find option by name */
            for (u32 i = 0; i < m_allocated_options.GetUsedCount(); ++i) {
                if (::strcmp(m_allocated_options[i]->option_name, option_name) == 0) {
                    return m_allocated_options[i]->argv_index;
                }
            }
        }
};
