#define OS_RELATED
#define BIT32_PROGRAM
#define KERNEL
#include <common.hpp>

extern uint8 filesystem[];
static uint16 indexx = 0;
static uint16 indexy = 0;

struct data
{
    uint64      address;
    uint64      region_length;
    uint32      region_type;
    uint32      extended_attributes;
} __attribute__((packed));

static uint16 *addr = (uint16 *) 0xB8000;

void print(uint8 *value)
{
    if(value[0] == '\n')
    {
        indexy++;
        indexx = 0;
        return;
    }

    uint8 i = 0;

    while(value[i])
    {
        if(indexx >= 80)
            {
                newline:
                indexy++;
                indexx = 0;
            }

        addr[indexx + (indexy * 80)] = (0x0F << 8) | (0xFF & value[i]);
        i++;
        indexx++;

        if(value[i] == '\n')
        {
            indexx = 0;
            while(value[i] == '\n')
            {
                i++;
                indexy++;
            }
        }

        if(value[i] == '\t')
        {
            while(value[i] == '\t')
            {
                indexx += 4;
                i++;
            }
        }
    }
}

void PrintHex(uint32 number)
{

	if(number <= 0)
	{
		uint8 *val = (uint8 *)"0x0";
		print((uint8 *)val);
		return;
	}

	uint8 hex[80];
	uint8 numbers[17] = "0123456789ABCDEF";
	uint8 i = 0, j = 0;

	while(number > 0)
	{
		hex[i] = numbers[(uint8)number & 0x0F];
		number >>= 4;
		i++;
	}


	hex[i++] = 'x';
	hex[i++] = '0';
	hex[i] = '\0';
	i--;

	for(j=0; j < i; j++, i--)
	{
		uint8 temp = hex[j];
		hex[j] = hex[i];
		hex[i] = temp;
	}
	print((uint8 *)hex);
}

void __attribute__((section("__start"))) main(void)
{
    unsigned char *f = (unsigned char *) 0xB8000;

    MemoryMap mem_map;
    mem_map.run_through_memory_map();

    if(filesystem[0] == 'F')
    {
        /* Do something. */
    }

    while(1);
}