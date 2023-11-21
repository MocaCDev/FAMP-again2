#ifndef FAMP_MEMORY_MAP_H
#define FAMP_MEMORY_MAP_H

extern uint8 memory_map[];
extern uint8 memory_map_entries[];

namespace FAMP_MemoryMap
{
    struct MemoryMap_Data
    {
        uint64      address;
        uint64      region_length;
        uint32      region_type;
        uint32      extended_attributes;
    } __attribute__((packed));

    enum class MemoryType
    {
        Available,
        Reserved,
        ACPI_Reclaimable,
        NVS,
        BadRam
    };

    struct MemoryMap_Data *FAMP_memory_map = (struct MemoryMap_Data *) memory_map;

    class MemoryMap
    {
    protected:
        uint32 available_memory = 0;
        uint32 total_memory = 0;

    public:
        MemoryMap() = default;

        void run_through_memory_map()
        {
            for(uint32 i = 0; i < *memory_map_entries; i++)
            {
                if(FAMP_memory_map->region_type == 1)
                {
                    if(FAMP_memory_map->address != 0 || i == 0)
                    {
                        available_memory += FAMP_memory_map->region_length;
                        total_memory += FAMP_memory_map->region_length;
                        print((uint8 *)"(FREE) Length: ");
                        PrintHex(FAMP_memory_map->region_length);
                        print((uint8 *)", Address: ");
                        if(FAMP_memory_map->address >= 0xFFFFFFFF)
                            PrintHex(FAMP_memory_map->address-1);
                        else
                            PrintHex(FAMP_memory_map->address);
                        //print((uint8 *)", Extra Attr: ");
                        //PrintHex(FAMP_memory_map->extended_attributes);
                        print((uint8 *)"\n");
                    }
                }

                if(FAMP_memory_map->region_type == 2)
                {
                    total_memory += FAMP_memory_map->region_length;
                    
                    print((uint8 *)"(RESERVED) Length: ");
                    PrintHex(FAMP_memory_map->region_length);
                    print((uint8 *)", Address: ");
                    PrintHex(FAMP_memory_map->address);
                    print((uint8 *)"\n");
                }
                    
                FAMP_memory_map++;
            }

            FAMP_memory_map-= *memory_map_entries;

            print((uint8 *)"Total (Free) Bytes: ");
            PrintHex(available_memory);
            print((uint8 *)". Total (Free/Reserved) Bytes: ");
            PrintHex(total_memory);
        }

        ~MemoryMap() = default;
    };
}

#endif