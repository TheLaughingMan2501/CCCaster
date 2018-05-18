#include "MemDump.hpp"
#include "Constants_ST.hpp"

#include <utility>
#include <algorithm>

using namespace std;


#define LOG_FILE "generator.log"


#define CC_P1_EXTRA_STRUCT_ADDR     ( ( char * )     0x5BF000 ) //( ( char * )     0x557DB8 )
#define CC_P2_EXTRA_STRUCT_ADDR     ( ( char * )     0x5BF20C ) //( ( char * )     0x557FC4 )
#define CC_EXTRA_STRUCT_SIZE        ( 0x20C )

#define CC_P1_SPELL_CIRCLE_ADDR     ( ( float * )    0x5CB3C8 ) //( ( float * )    0x5641A4 )
#define CC_P2_SPELL_CIRCLE_ADDR     ( ( float * )    0x5CB424 ) //( ( float * )    0x564200 )

#define CC_METER_ANIMATION_ADDR     ( ( uint32_t * ) 0x7D8C74 ) //( ( uint32_t * ) 0x7717D8 )

#define CC_EFFECTS_ARRAY_ADDR       ( ( char * )     0x6E28F0 ) //( ( char * )     0x67BDE8 )
#define CC_EFFECTS_ARRAY_COUNT      ( 1000 )
#define CC_EFFECT_ELEMENT_SIZE      ( 0x33C )

#define CC_SUPER_FLASH_PAUSE_ADDR   ( ( uint32_t * ) 0x5BF858 ) //( ( uint32_t * ) 0x5595B4 )
#define CC_SUPER_FLASH_TIMER_ADDR   ( ( uint32_t * ) 0x5C9C60 ) //( ( uint32_t * ) 0x562A48 )

#define CC_SUPER_STATE_ARRAY_ADDR   ( ( char * )     0x5BF860 ) //( ( char * )     0x558608 )
#define CC_SUPER_STATE_ARRAY_SIZE   ( 5 * 0x30C )

#define CC_P1_STATUS_MSG_ARRAY_ADDR ( ( char * )     0x5CA7B0 ) //( ( char * )     0x563580 )
#define CC_P2_STATUS_MSG_ARRAY_ADDR ( ( char * )     0x5CA824 ) //( ( char * )     0x5635F4 )
#define CC_STATUS_MSG_ARRAY_SIZE    ( 0x60 )

#define CC_CAMERA_SCALE_1_ADDR      ( ( float * )    0x5B3B90 ) //( ( float * )    0x54EB70 )// zoom
#define CC_CAMERA_SCALE_2_ADDR      ( ( float * )    0x5B3B94 ) //( ( float * )    0x54EB74 )// zoom
#define CC_CAMERA_SCALE_3_ADDR      ( ( float * )    0x5B3A28 ) //( ( float * )    0x54EB78 )

#define CC_INPUT_STATE_ADDR         ( ( uint8_t * )  0x5C513F ) //( ( uint8_t * )  0x562A6F )// TODO figure out what the values mean
#define CC_SLOW_TIMER_INIT_ADDR     ( ( uint16_t * ) 0x5C9C94 ) //( ( uint16_t * ) 0x562A6C )// Initializes the slowdown timer
#define CC_SLOW_TIMER_ADDR          ( ( uint16_t * ) 0x5C444C ) //( ( uint16_t * ) 0x55D208 )// Slowdown timer

#define CC_GRAPHICS_ARRAY_ADDR      ( ( char * )     0x684CE0 ) //( ( char * )     0x61E170 )
#define CC_GRAPHICS_ARRAY_SIZE      ( 4000 * 0x60 )

#define CC_GRAPHICS_COUNTER         ( ( uint32_t * ) 0x684CB4 ) //( ( uint32_t * ) 0x67BD78 )


static const vector<MemDump> playerAddrs =
{
	{ 0x5BC370, 0x5BC380 }, //{ 0x555130, 0x555140 }, // ??? 0x555130 1 byte: some timer flag
    { 0x5BC380, 0x5BC3A0 }, //{ 0x555140, 0x555160 },
    { 0x5BC3A0, 0x5BC3C0 }, //{ 0x555160, 0x555180 },// ???
    { 0x5BC3C0, 0x5BC3C8 }, //{ 0x555180, 0x555188 },
    { 0x5BC3C8, 0x5BC3D0 }, //{ 0x555188, 0x555190 },// ???
    { 0x5BC3D0, 0x5BC480 }, //{ 0x555190, 0x555240 },
    ( uint32_t * ) 0x5BC480, //( uint32_t * ) 0x555240,// ???
    { 0x5BC484, 0x5BC4C4 }, //{ 0x555244, 0x555284 },
    ( uint32_t * ) 0x5BC4C4, //( uint32_t * ) 0x555284,// ???
    { 0x5BC4C8, 0x5BC52C }, //{ 0x555288, 0x5552EC },
    ( uint32_t * ) 0x5BC52C, //( uint32_t * ) 0x5552EC,// ???
    { 0x5BC530, 0x5BC534 }, //{ 0x5552F0, 0x5552F4 },
    { 0x5BC534, 0x5BC550 }, //{ 0x5552F4, 0x555310 },// ??? 0x5552F6, 2 bytes: Sion bullets, inverse counter
    { 0x5BC550, 0x5BC56C }, //{ 0x555310, 0x55532C },

    { ( void * ) 0x5BC56C, 4 }, //{ ( void * ) 0x55532C, 4 },
    // { ( void * ) 0x55532C, 4, {
    //     MemDumpPtr ( 0, 0x24, 1 ), // segfaulted on this once
    //     MemDumpPtr ( 0, 0x30, 2 ),
    // } },

    { 0x5BC570, 0x5BC58C }, //{ 0x555330, 0x55534C },// ???
    { 0x5BC58C, 0x5BC59C }, //{ 0x55534C, 0x55535C },
    { 0x5BC59C, 0x5BC60C }, //{ 0x55535C, 0x5553CC },// ???

    { ( void * ) 0x5BC60C, 4 }, //{ ( void * ) 0x5553CC, 4 }// pointer to player struct?

    { 0x5BC610, 0x5BC62C }, //{ 0x5553D0, 0x5553EC },// ???

    { ( void * ) 0x5BC62C, 4 }, //{ ( void * ) 0x5553EC, 4 }, // pointer to player struct?
    { ( void * ) 0x5BC630, 4 }, //{ ( void * ) 0x5553F0, 4 }, // pointer to player struct?

    { 0x5BC634, 0x5BC63C }, //{ 0x5553F4, 0x5553FC },

    { ( void * ) 0x5BC63C, 4 }, //{ ( void * ) 0x5553FC, 4 }, // pointer to player struct?
    { ( void * ) 0x5BC640, 4 }, //{ ( void * ) 0x555400, 4 }, // pointer to player struct?

    { 0x5BC644, 0x5BC650 }, //{ 0x555404, 0x555410 }, // ???
    { 0x5BC650, 0x5BC66C }, //{ 0x555410, 0x55542C },
    ( uint32_t * ) 0x5BC66C, //( uint32_t * ) 0x55542C, // ???
    { 0x5BC670, 0x5BC68C }, //{ 0x555430, 0x55544C },

    { ( void * ) 0x5BC68C, 4 }, //{ ( void * ) 0x55544C, 4 }, // graphics pointer? this is accessed all the time even when paused

    { ( void * ) 0x5BC690, 4 }, //{ ( void * ) 0x555450, 4 }, // graphics pointer? this is accessed all the time even when paused
    // { ( void * ) 0x555450, 4, {
    //     MemDumpPtr ( 0, 0x00, 2 ),
    //     MemDumpPtr ( 0, 0x0C, 2 ),
    //     MemDumpPtr ( 0, 0x0E, 1 ),
    //     MemDumpPtr ( 0, 0x0F, 1 ),
    //     MemDumpPtr ( 0, 0x10, 2 ),
    //     MemDumpPtr ( 0, 0x12, 2 ),
    //     MemDumpPtr ( 0, 0x16, 2 ),
    //     MemDumpPtr ( 0, 0x1B, 1 ),
    //     MemDumpPtr ( 0, 0x1C, 1 ),
    //     MemDumpPtr ( 0, 0x2E, 2 ),
    //     MemDumpPtr ( 0, 0x38, 4, {
    //         MemDumpPtr ( 0, 0x00, 4, {
    //             MemDumpPtr ( 0, 0x00, 1 ),
    //             MemDumpPtr ( 0, 0x02, 2 ),
    //             MemDumpPtr ( 0, 0x04, 2 ),
    //             MemDumpPtr ( 0, 0x06, 1 ),
    //             MemDumpPtr ( 0, 0x08, 1 ),
    //         } ),
    //         MemDumpPtr ( 0, 0x08, 4, {
    //             MemDumpPtr ( 0, 0x00, 1 ),
    //             MemDumpPtr ( 0, 0x02, 1 ),
    //             MemDumpPtr ( 0, 0x06, 2 ),
    //             MemDumpPtr ( 0, 0x0C, 4 ),
    //         } ),
    //         MemDumpPtr ( 0, 0x0C, 1 ),
    //         MemDumpPtr ( 0, 0x11, 1 ),
    //         MemDumpPtr ( 0, 0x14, 1 ),
    //     } ),
    //     MemDumpPtr ( 0, 0x40, 1 ),
    //     MemDumpPtr ( 0, 0x41, 1 ),
    //     MemDumpPtr ( 0, 0x42, 1 ),
    //     MemDumpPtr ( 0, 0x44, 4 ), // more to this?
    //     MemDumpPtr ( 0, 0x4C, 4, {
    //         MemDumpPtr ( 0, 0, 4, {
    //             MemDumpPtr ( 0, 0x00, 2 ),
    //             MemDumpPtr ( 0, 0x02, 2 ),
    //             MemDumpPtr ( 0, 0x04, 2 ),
    //             MemDumpPtr ( 0, 0x06, 2 ),
    //         } ),
    //     } ),
    // } },

	{ ( void * ) 0x5BC694, 4 }, //{ ( void * ) 0x555454, 4 }, // graphics pointer? this is accessed all the time even when paused

    { ( void * ) 0x5BC698, 4 }, //{ ( void * ) 0x555458, 4 }, // pointer to player struct?

	{ 0x5BC69C, 0x5BC6A0 }, //{ 0x55545C, 0x555460 },

    // graphics pointer(s)? these are accessed all the time even when paused
    { ( void * ) 0x5BC6A0, 4 }, //{ ( void * ) 0x555460, 4 },
    // { ( void * ) 0x555460, 4, {
    //     MemDumpPtr ( 0, 0x0, 4, {
    //         MemDumpPtr ( 0, 0x4, 4, {
    //             MemDumpPtr ( 0, 0xC, 4 )
    //         } )
    //     } )
    // } },

    { 0x5BC6A4, 0x5BC6AC }, //{ 0x555464, 0x55546C },

    { ( void * ) 0x5BC6AC, 4 }, //{ ( void * ) 0x55546C, 4 },// graphics pointer? this is accessed all the time even when paused

    { 0x5BC6B0, 0x5BC74C }, //{ 0x555470, 0x55550C },
    ( uint32_t * )0x5BC74C, //( uint32_t * ) 0x55550C,// ???
    { 0x5BC750, 0x5BC758 }, //{ 0x555510, 0x555518 },

    { 0x5BC758, 0x5BC85A }, //{ 0x555518, 0x55561A }, // input history (directions)
    { 0x5BC85A, 0x5BC95C }, //{ 0x55561A, 0x55571C }, // input history (A button)
    { 0x5BC95C, 0x5BCA5E }, //{ 0x55571C, 0x55581E }, // input history (B button)
    { 0x5BCA5E, 0x5BCB60 }, //{ 0x55581E, 0x555920 },// input history (C button)
    { 0x5BCB60, 0x5BCC62 }, //{ 0x555920, 0x555A22 },// input history (D button)
    { 0x5BCC62, 0x5BCD64 }, //{ 0x555A22, 0x555B24 },// input history (E button)

    { 0x5BCD64, 0x5BCD6C }, //{ 0x555B24, 0x555B2C },
    { 0x5BCD6C, 0x5BCE6C }, //{ 0x555B2C, 0x555C2C },// ???
};

static const vector<MemDump> miscAddrs =
{
    // The stack range before calling the main dll callback
    // { 0x18FEA0, 0x190000 },

    // Game state
    CC_ROUND_TIMER_ADDR,
    CC_REAL_TIMER_ADDR,
    CC_WORLD_TIMER_ADDR,
    CC_SLOW_TIMER_INIT_ADDR,
    CC_SLOW_TIMER_ADDR,
    CC_INTRO_STATE_ADDR,
    CC_INPUT_STATE_ADDR,
    CC_SKIPPABLE_FLAG_ADDR,

    CC_RNG_STATE0_ADDR,
    CC_RNG_STATE1_ADDR,
    CC_RNG_STATE2_ADDR,
    { CC_RNG_STATE3_ADDR, CC_RNG_STATE3_SIZE },

    // Unknown states
    ( uint32_t * ) 0x5CAAB4, //( uint32_t * ) 0x563864,
    ( uint32_t * ) 0x5CA9C4, //( uint32_t * ) 0x56414C,

    // Graphical effects
    { CC_GRAPHICS_ARRAY_ADDR, CC_GRAPHICS_ARRAY_SIZE },
    CC_GRAPHICS_COUNTER,

    CC_SUPER_FLASH_PAUSE_ADDR,
    CC_SUPER_FLASH_TIMER_ADDR,

    { CC_SUPER_STATE_ARRAY_ADDR, CC_SUPER_STATE_ARRAY_SIZE },

    // Player state
    { CC_P1_EXTRA_STRUCT_ADDR, CC_EXTRA_STRUCT_SIZE },
    { CC_P2_EXTRA_STRUCT_ADDR, CC_EXTRA_STRUCT_SIZE },

    CC_P1_WINS_ADDR,
    CC_P2_WINS_ADDR,

    CC_P1_GAME_POINT_FLAG_ADDR,
    CC_P2_GAME_POINT_FLAG_ADDR,

    // HUD misc graphics
    CC_METER_ANIMATION_ADDR,
    CC_P1_SPELL_CIRCLE_ADDR,
    CC_P2_SPELL_CIRCLE_ADDR,

    // HUD status message graphics
    { CC_P1_STATUS_MSG_ARRAY_ADDR, CC_STATUS_MSG_ARRAY_SIZE },
    { CC_P2_STATUS_MSG_ARRAY_ADDR, CC_STATUS_MSG_ARRAY_SIZE },

    // Intro / outro graphics
    ( uint32_t * ) 0x7B4540, //( uint32_t * ) 0x74D9D0,
    ( uint32_t * ) 0x7B4AB8, //( uint32_t * ) 0x74E4E4,
    ( float * ) 0x7B4ABC, //( float * ) 0x74E4E8,
    // ( uint32_t * ) 0x76E79C,

    // Intro graphics/music/voice
    ( uint32_t * ) 0x7B40A4,//( uint32_t * ) 0x74D598,
    ( uint32_t * ) 0x7B4B8C,//( uint32_t * ) 0x74E5B0,
    ( uint32_t * ) 0x7B4D70,//( uint32_t * ) 0x74E768,
    { 0x7B4D78, 0x7B4D8C },//{ 0x74E770, 0x74E784 },
    { 0x7B4D94, 0x7B4DA0 },//{ 0x74E78C, 0x74E798 },
    { 0x7B4DA4, 0x7B4DAC },//{ 0x74E79C, 0x74E7A8 },
    { 0x7B4DB4, 0x7B4DC8 },//{ 0x74E7AC, 0x74E7C0 },
    { 0x7B4DD0, 0x7B4DE0 },//{ 0x74E7C8, 0x74E7D8 },
    { 0x7B4DE4, 0x7B4DE8 },//{ 0x74E7DC, 0x74E7E0 },
    { 0x7B4DEC, 0x7B4DFC },//{ 0x74E7E4, 0x74E7F4 },
    { 0x7B4E00, 0x7B4E10 },//{ 0x74E7F8, 0x74E808 },
    { 0x7B4E14, 0x7B4E18 },//{ 0x74E80C, 0x74E810 },
    { 0x7B4E1C, 0x7B4E30 },//{ 0x74E814, 0x74E828 },
    { 0x7B4E34, 0x7B4E3C },//{ 0x74E82C, 0x74E834 },
    { 0x7B4E40, 0x7B4E54 },//{ 0x74E838, 0x74E84C },
    { 0x7B4E58, 0x7B4E60 },//{ 0x74E850, 0x74E858 },
    { 0x7B4E64, 0x7B4E74 },//{ 0x74E85C, 0x74E86C },

    // Intro graphics state part 2
	( uint32_t *) 0x7B4AB0, //{ 0x76E780, 0x76E78C },

    // Camera position state
    ( uint32_t * ) 0x5BC35C,//( uint32_t * ) 0x555124,
    ( uint32_t * ) 0x5BC360,//( uint32_t * ) 0x555128, (‰ö‚µ‚¢)->Œ©’¼‚µ
    { 0x5BF834, 0x5BF840 }, //{ 0x5585E8, 0x5585F4 },
	//( uint32_t * ) 0x5C9C84,  //{ 0x55DEC4, 0x55DED0 },Œ©’¼‚µ
	( uint32_t * ) 0x5C510C,
    { 0x5C511C, 0x5C5120 }, //{ 0x55DEDC, 0x55DEE8 },
    { 0x5C9C84, 0x5C9C90 }, //{ 0x564B14, 0x564B20 },(‰ö‚µ‚¢)->Œ©’¼‚µ

    // More camera position state
    ( uint16_t * ) 0x5CB648, //( uint16_t * ) 0x564B10,
    ( uint32_t * ) 0x5CA7A0, //( uint32_t * ) 0x563750,
    ( uint32_t * ) 0x5BEFF4, //( uint32_t * ) 0x557DB0,
    ( uint32_t * ) 0x5BEFF8, //( uint32_t * ) 0x557DB4

    ( uint8_t * )  0x5BB7F3, //( uint8_t * ) 0x557D2B,(‰ö‚µ‚¢)->Œ©’¼‚µ
    ( uint16_t * ) 0x5BEFFC, //( uint16_t * ) 0x557DAC,
    ( uint16_t * ) 0x5BEF6B, //( uint16_t * ) 0x559546,
    ( uint16_t * ) 0x5CB62C, //( uint16_t * ) 0x564B00,
    ( uint32_t * ) 0x5CB61C, //( uint32_t * ) 0x76E6F8,
    ( uint32_t * ) 0x5CB650, //( uint32_t * ) 0x76E6FC,
    ( uint32_t * ) 0x5BA028, //( uint32_t * ) 0x7B1D2C,

    // Camera scaling state
    ( uint32_t * ) 0x5C4448, //( uint32_t * ) 0x55D204,
    ( uint32_t * ) 0x5CA7A4, //( uint32_t * ) 0x56357C,
    ( uint32_t * ) 0x5C5120, //( uint32_t * ) 0x55DEE8,
    ( uint32_t * ) 0x5CB638, //( uint32_t * ) 0x564B0C,
    ( uint32_t * ) 0x5CB620, //( uint32_t * ) 0x564AF8,
    ( uint32_t * ) 0x5CB654, //( uint32_t * ) 0x564B24,
    ( uint32_t * ) 0x5CB628, //( uint32_t * ) 0x76E6F4,

    CC_CAMERA_SCALE_1_ADDR,
    CC_CAMERA_SCALE_2_ADDR,
    CC_CAMERA_SCALE_3_ADDR,
};

static const MemDump firstEffect ( CC_EFFECTS_ARRAY_ADDR, CC_EFFECT_ELEMENT_SIZE, {
    MemDumpPtr ( 0x320, 0x38, 4, {
        MemDumpPtr ( 0, 0, 4, {
            MemDumpPtr ( 0, 0, 4 )
        } )
    } )
} );


int main ( int argc, char *argv[] )
{
    if ( argc < 2 )
    {
        PRINT ( "No output file specified!" );
        return -1;
    }

    Logger::get().initialize ( LOG_FILE, 0 );

    MemDumpList allAddrs;

    allAddrs.append ( miscAddrs );

    allAddrs.append ( playerAddrs );                            // Player 1
    allAddrs.append ( playerAddrs, CC_PLR_STRUCT_SIZE );        // Player 2
    allAddrs.append ( playerAddrs, 2 * CC_PLR_STRUCT_SIZE );    // Puppet 1
    allAddrs.append ( playerAddrs, 3 * CC_PLR_STRUCT_SIZE );    // Puppet 2

    for ( size_t i = 0; i < CC_EFFECTS_ARRAY_COUNT; ++i )
        allAddrs.append ( firstEffect, CC_EFFECT_ELEMENT_SIZE * i );

    allAddrs.update();

    LOG ( "allAddrs.totalSize=%u", allAddrs.totalSize );

    LOG ( "allAddrs:" );
    for ( const MemDump& mem : allAddrs.addrs )
    {
        LOG ( "{ 0x%06X, 0x%06X }", mem.getAddr(), mem.getAddr() + mem.size );

        for ( const MemDumpPtr& ptr0 : mem.ptrs )
        {
            ASSERT ( ptr0.parent == &mem );

            LOG ( "  [0x%x]+0x%x -> { %u bytes }", ptr0.srcOffset, ptr0.dstOffset, ptr0.size );

            for ( const MemDumpPtr& ptr1 : ptr0.ptrs )
            {
                ASSERT ( ptr1.parent == &ptr0 );

                LOG ( "    [0x%x]+0x%x -> { %u bytes }", ptr1.srcOffset, ptr1.dstOffset, ptr1.size );

                for ( const MemDumpPtr& ptr2 : ptr1.ptrs )
                {
                    ASSERT ( ptr2.parent == &ptr1 );

                    LOG ( "      [0x%x]+0x%x -> { %u bytes }", ptr2.srcOffset, ptr2.dstOffset, ptr2.size );
                }
            }
        }
    }

    allAddrs.save ( argv[1] );

    Logger::get().deinitialize();
    return 0;
}
