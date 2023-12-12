#include <Arduino.h>
#include <limits.h>

class WaveGenerator {
private:
    struct Note {
        uint32_t phase;
        uint32_t phase_delta;
        bool active;
        uint8_t actnum;
        uint8_t note;
    };

    static const int MAX_NOTES = 4; // 6音目からおかしくなる
    Note notes[MAX_NOTES];
    float volume_gain;
    const int32_t sample_rate;
    uint8_t preset = 0x00;

    ///////////////////////////TODO////////////////////////////////
    int16_t saw[513] = {+0,  +7096,  +9594,  +8312,  +7268,  +7934,  +8542,  +8030,  +7526,  +7858,  +8201,  +7878,  +7533,  +7743,  +7980,  +7742,+7474,  +7620,  +7799,  +7610,  +7387,  +7495,  +7638,  +7480,  +7287,  +7368,  +7486,  +7351,  +7179,  +7241,  +7341,  +7223,+7066,  +7114,  +7200,  +7094,  +6949,  +6986,  +7062,  +6966,  +6831,  +6858,  +6925,  +6838,  +6710,  +6731,  +6790,  +6709,+6589,  +6603,  +6656,  +6581,  +6466,  +6475,  +6523,  +6453,  +6343,  +6347,  +6391,  +6325,  +6219,  +6219,  +6259,  +6197,+6095,  +6091,  +6128,  +6069,  +5970,  +5963,  +5996,  +5941,  +5845,  +5835,  +5866,  +5813,  +5720,  +5707,  +5735,  +5685,+5594,  +5579,  +5605,  +5557,  +5468,  +5451,  +5475,  +5429,  +5342,  +5323,  +5345,  +5301,  +5216,  +5195,  +5215,  +5173,+5090,  +5067,  +5086,  +5045,  +4963,  +4939,  +4956,  +4917,  +4837,  +4811,  +4827,  +4789,  +4710,  +4683,  +4697,  +4661,+4583,  +4555,  +4568,  +4533,  +4456,  +4427,  +4439,  +4405,  +4330,  +4299,  +4310,  +4277,  +4203,  +4171,  +4181,  +4149,+4076,  +4043,  +4052,  +4021,  +3949,  +3915,  +3923,  +3893,  +3822,  +3787,  +3794,  +3765,  +3694,  +3659,  +3665,  +3637,+3567,  +3531,  +3536,  +3509,  +3440,  +3404,  +3407,  +3380,  +3313,  +3276,  +3279,  +3252,  +3186,  +3148,  +3150,  +3124,+3058,  +3020,  +3021,  +2996,  +2931,  +2892,  +2893,  +2868,  +2804,  +2764,  +2764,  +2740,  +2676,  +2636,  +2635,  +2612,+2549,  +2508,  +2507,  +2484,  +2422,  +2380,  +2378,  +2356,  +2294,  +2252,  +2249,  +2228,  +2167,  +2124,  +2121,  +2100,+2040,  +1996,  +1992,  +1972,  +1912,  +1868,  +1864,  +1844,  +1785,  +1740,  +1735,  +1716,  +1657,  +1612,  +1606,  +1588,+1530,  +1484,  +1478,  +1460,  +1402,  +1356,  +1349,  +1332,  +1275,  +1228,  +1221,  +1204,  +1147,  +1100,  +1092,  +1076,+1020,   +972,   +964,   +948,   +892,   +844,   +835,   +820,   +765,   +716,   +707,   +692,   +637,   +588,   +578,   +564,+510,   +460,   +450,   +436,   +382,   +332,   +321,   +308,   +255,   +204,   +193,   +180,   +127,    +76,    +64,    +52,+0,    -52,    -64,    -76,   -127,   -180,   -193,   -204,   -255,   -308,   -321,   -332,   -382,   -436,   -450,   -460,-510,   -564,   -578,   -588,   -637,   -692,   -707,   -716,   -765,   -820,   -835,   -844,   -892,   -948,   -964,   -972,-1020,  -1076,  -1092,  -1100,  -1147,  -1204,  -1221,  -1228,  -1275,  -1332,  -1349,  -1356,  -1402,  -1460,  -1478,  -1484,-1530,  -1588,  -1606,  -1612,  -1657,  -1716,  -1735,  -1740,  -1785,  -1844,  -1864,  -1868,  -1912,  -1972,  -1992,  -1996,-2040,  -2100,  -2121,  -2124,  -2167,  -2228,  -2249,  -2252,  -2294,  -2356,  -2378,  -2380,  -2422,  -2484,  -2507,  -2508,-2549,  -2612,  -2635,  -2636,  -2676,  -2740,  -2764,  -2764,  -2804,  -2868,  -2893,  -2892,  -2931,  -2996,  -3021,  -3020,-3058,  -3124,  -3150,  -3148,  -3186,  -3252,  -3279,  -3276,  -3313,  -3380,  -3407,  -3404,  -3440,  -3509,  -3536,  -3531,-3567,  -3637,  -3665,  -3659,  -3694,  -3765,  -3794,  -3787,  -3822,  -3893,  -3923,  -3915,  -3949,  -4021,  -4052,  -4043,-4076,  -4149,  -4181,  -4171,  -4203,  -4277,  -4310,  -4299,  -4330,  -4405,  -4439,  -4427,  -4456,  -4533,  -4568,  -4555,-4583,  -4661,  -4697,  -4683,  -4710,  -4789,  -4827,  -4811,  -4837,  -4917,  -4956,  -4939,  -4963,  -5045,  -5086,  -5067,-5090,  -5173,  -5215,  -5195,  -5216,  -5301,  -5345,  -5323,  -5342,  -5429,  -5475,  -5451,  -5468,  -5557,  -5605,  -5579,-5594,  -5685,  -5735,  -5707,  -5720,  -5813,  -5866,  -5835,  -5845,  -5941,  -5996,  -5963,  -5970,  -6069,  -6128,  -6091,-6095,  -6197,  -6259,  -6219,  -6219,  -6325,  -6391,  -6347,  -6343,  -6453,  -6523,  -6475,  -6466,  -6581,  -6656,  -6603,-6589,  -6709,  -6790,  -6731,  -6710,  -6838,  -6925,  -6858,  -6831,  -6966,  -7062,  -6986,  -6949,  -7094,  -7200,  -7114,-7066,  -7223,  -7341,  -7241,  -7179,  -7351,  -7486,  -7368,  -7287,  -7480,  -7638,  -7495,  -7387,  -7610,  -7799,  -7620,-7474,  -7742,  -7980,  -7743,  -7533,  -7878,  -8201,  -7858,  -7526,  -8030,  -8542,  -7934,  -7268,  -8312,  -9594,  -7096,+0};

    int16_t square[513] = {+0,  +7149,  +9658,  +8388,  +7396,  +8114,  +8735,  +8233,  +7781,  +8167,  +8522,  +8209,  +7916,  +8180,  +8429,  +8201,+7984,  +8185,  +8378,  +8198,  +8025,  +8187,  +8345,  +8196,  +8052,  +8189,  +8322,  +8195,  +8071,  +8190,  +8305,  +8194,+8086,  +8190,  +8292,  +8194,  +8097,  +8191,  +8283,  +8193,  +8106,  +8191,  +8275,  +8193,  +8113,  +8191,  +8268,  +8193,+8119,  +8191,  +8263,  +8193,  +8124,  +8191,  +8258,  +8193,  +8128,  +8191,  +8254,  +8193,  +8131,  +8191,  +8251,  +8192,+8134,  +8192,  +8248,  +8192,  +8137,  +8192,  +8246,  +8192,  +8139,  +8192,  +8244,  +8192,  +8141,  +8192,  +8242,  +8192,+8143,  +8192,  +8240,  +8192,  +8145,  +8192,  +8239,  +8192,  +8146,  +8192,  +8238,  +8192,  +8147,  +8192,  +8237,  +8192,+8148,  +8192,  +8236,  +8192,  +8149,  +8192,  +8235,  +8192,  +8149,  +8192,  +8234,  +8192,  +8150,  +8192,  +8234,  +8192,+8150,  +8192,  +8233,  +8192,  +8151,  +8192,  +8233,  +8192,  +8151,  +8192,  +8233,  +8192,  +8151,  +8192,  +8233,  +8192,+8151,  +8192,  +8233,  +8192,  +8151,  +8192,  +8233,  +8192,  +8151,  +8192,  +8233,  +8192,  +8151,  +8192,  +8233,  +8192,+8150,  +8192,  +8234,  +8192,  +8150,  +8192,  +8234,  +8192,  +8149,  +8192,  +8235,  +8192,  +8149,  +8192,  +8236,  +8192,+8148,  +8192,  +8237,  +8192,  +8147,  +8192,  +8238,  +8192,  +8146,  +8192,  +8239,  +8192,  +8145,  +8192,  +8240,  +8192,+8143,  +8192,  +8242,  +8192,  +8141,  +8192,  +8244,  +8192,  +8139,  +8192,  +8246,  +8192,  +8137,  +8192,  +8248,  +8192,+8134,  +8192,  +8251,  +8191,  +8131,  +8193,  +8254,  +8191,  +8128,  +8193,  +8258,  +8191,  +8124,  +8193,  +8263,  +8191,+8119,  +8193,  +8268,  +8191,  +8113,  +8193,  +8275,  +8191,  +8106,  +8193,  +8283,  +8191,  +8097,  +8194,  +8292,  +8190,+8086,  +8194,  +8305,  +8190,  +8071,  +8195,  +8322,  +8189,  +8052,  +8196,  +8345,  +8187,  +8025,  +8198,  +8378,  +8185,+7984,  +8201,  +8429,  +8180,  +7916,  +8209,  +8522,  +8167,  +7781,  +8233,  +8735,  +8114,  +7396,  +8388,  +9658,  +7149,+0,  -7149,  -9658,  -8388,  -7396,  -8114,  -8735,  -8233,  -7781,  -8167,  -8522,  -8209,  -7916,  -8180,  -8429,  -8201,-7984,  -8185,  -8378,  -8198,  -8025,  -8187,  -8345,  -8196,  -8052,  -8189,  -8322,  -8195,  -8071,  -8190,  -8305,  -8194,-8086,  -8190,  -8292,  -8194,  -8097,  -8191,  -8283,  -8193,  -8106,  -8191,  -8275,  -8193,  -8113,  -8191,  -8268,  -8193,-8119,  -8191,  -8263,  -8193,  -8124,  -8191,  -8258,  -8193,  -8128,  -8191,  -8254,  -8193,  -8131,  -8191,  -8251,  -8192,-8134,  -8192,  -8248,  -8192,  -8137,  -8192,  -8246,  -8192,  -8139,  -8192,  -8244,  -8192,  -8141,  -8192,  -8242,  -8192,-8143,  -8192,  -8240,  -8192,  -8145,  -8192,  -8239,  -8192,  -8146,  -8192,  -8238,  -8192,  -8147,  -8192,  -8237,  -8192,-8148,  -8192,  -8236,  -8192,  -8149,  -8192,  -8235,  -8192,  -8149,  -8192,  -8234,  -8192,  -8150,  -8192,  -8234,  -8192,-8150,  -8192,  -8233,  -8192,  -8151,  -8192,  -8233,  -8192,  -8151,  -8192,  -8233,  -8192,  -8151,  -8192,  -8233,  -8192,-8151,  -8192,  -8233,  -8192,  -8151,  -8192,  -8233,  -8192,  -8151,  -8192,  -8233,  -8192,  -8151,  -8192,  -8233,  -8192,-8150,  -8192,  -8234,  -8192,  -8150,  -8192,  -8234,  -8192,  -8149,  -8192,  -8235,  -8192,  -8149,  -8192,  -8236,  -8192,-8148,  -8192,  -8237,  -8192,  -8147,  -8192,  -8238,  -8192,  -8146,  -8192,  -8239,  -8192,  -8145,  -8192,  -8240,  -8192,-8143,  -8192,  -8242,  -8192,  -8141,  -8192,  -8244,  -8192,  -8139,  -8192,  -8246,  -8192,  -8137,  -8192,  -8248,  -8192,-8134,  -8192,  -8251,  -8191,  -8131,  -8193,  -8254,  -8191,  -8128,  -8193,  -8258,  -8191,  -8124,  -8193,  -8263,  -8191,-8119,  -8193,  -8268,  -8191,  -8113,  -8193,  -8275,  -8191,  -8106,  -8193,  -8283,  -8191,  -8097,  -8194,  -8292,  -8190,-8086,  -8194,  -8305,  -8190,  -8071,  -8195,  -8322,  -8189,  -8052,  -8196,  -8345,  -8187,  -8025,  -8198,  -8378,  -8185,-7984,  -8201,  -8429,  -8180,  -7916,  -8209,  -8522,  -8167,  -7781,  -8233,  -8735,  -8114,  -7396,  -8388,  -9658,  -7149,+0,};

    int16_t sine[513] = {+0,   +101,   +201,   +302,   +402,   +502,   +603,   +703,   +803,   +903,  +1003,  +1102,  +1202,  +1301,  +1401,  +1499,+1598,  +1697,  +1795,  +1893,  +1990,  +2088,  +2185,  +2282,  +2378,  +2474,  +2570,  +2665,  +2760,  +2854,  +2948,  +3042,+3135,  +3228,  +3320,  +3411,  +3503,  +3593,  +3683,  +3773,  +3862,  +3950,  +4038,  +4125,  +4212,  +4297,  +4383,  +4467,+4551,  +4634,  +4717,  +4799,  +4880,  +4960,  +5040,  +5119,  +5197,  +5274,  +5351,  +5427,  +5501,  +5575,  +5649,  +5721,+5793,  +5863,  +5933,  +6002,  +6070,  +6137,  +6203,  +6268,  +6333,  +6396,  +6458,  +6519,  +6580,  +6639,  +6698,  +6755,+6811,  +6867,  +6921,  +6974,  +7027,  +7078,  +7128,  +7177,  +7225,  +7272,  +7317,  +7362,  +7405,  +7448,  +7489,  +7529,+7568,  +7606,  +7643,  +7679,  +7713,  +7746,  +7779,  +7809,  +7839,  +7868,  +7895,  +7921,  +7946,  +7970,  +7993,  +8014,+8035,  +8054,  +8071,  +8088,  +8103,  +8117,  +8130,  +8142,  +8153,  +8162,  +8170,  +8177,  +8182,  +8186,  +8190,  +8191,+8192,  +8191,  +8190,  +8186,  +8182,  +8177,  +8170,  +8162,  +8153,  +8142,  +8130,  +8117,  +8103,  +8088,  +8071,  +8054,+8035,  +8014,  +7993,  +7970,  +7946,  +7921,  +7895,  +7868,  +7839,  +7809,  +7779,  +7746,  +7713,  +7679,  +7643,  +7606,+7568,  +7529,  +7489,  +7448,  +7405,  +7362,  +7317,  +7272,  +7225,  +7177,  +7128,  +7078,  +7027,  +6974,  +6921,  +6867,+6811,  +6755,  +6698,  +6639,  +6580,  +6519,  +6458,  +6396,  +6333,  +6268,  +6203,  +6137,  +6070,  +6002,  +5933,  +5863,+5793,  +5721,  +5649,  +5575,  +5501,  +5427,  +5351,  +5274,  +5197,  +5119,  +5040,  +4960,  +4880,  +4799,  +4717,  +4634,+4551,  +4467,  +4383,  +4297,  +4212,  +4125,  +4038,  +3950,  +3862,  +3773,  +3683,  +3593,  +3503,  +3411,  +3320,  +3228,+3135,  +3042,  +2948,  +2854,  +2760,  +2665,  +2570,  +2474,  +2378,  +2282,  +2185,  +2088,  +1990,  +1893,  +1795,  +1697,+1598,  +1499,  +1401,  +1301,  +1202,  +1102,  +1003,   +903,   +803,   +703,   +603,   +502,   +402,   +302,   +201,   +101,+0,   -101,   -201,   -302,   -402,   -502,   -603,   -703,   -803,   -903,  -1003,  -1102,  -1202,  -1301,  -1401,  -1499,-1598,  -1697,  -1795,  -1893,  -1990,  -2088,  -2185,  -2282,  -2378,  -2474,  -2570,  -2665,  -2760,  -2854,  -2948,  -3042,-3135,  -3228,  -3320,  -3411,  -3503,  -3593,  -3683,  -3773,  -3862,  -3950,  -4038,  -4125,  -4212,  -4297,  -4383,  -4467,-4551,  -4634,  -4717,  -4799,  -4880,  -4960,  -5040,  -5119,  -5197,  -5274,  -5351,  -5427,  -5501,  -5575,  -5649,  -5721,-5793,  -5863,  -5933,  -6002,  -6070,  -6137,  -6203,  -6268,  -6333,  -6396,  -6458,  -6519,  -6580,  -6639,  -6698,  -6755,-6811,  -6867,  -6921,  -6974,  -7027,  -7078,  -7128,  -7177,  -7225,  -7272,  -7317,  -7362,  -7405,  -7448,  -7489,  -7529,-7568,  -7606,  -7643,  -7679,  -7713,  -7746,  -7779,  -7809,  -7839,  -7868,  -7895,  -7921,  -7946,  -7970,  -7993,  -8014,-8035,  -8054,  -8071,  -8088,  -8103,  -8117,  -8130,  -8142,  -8153,  -8162,  -8170,  -8177,  -8182,  -8186,  -8190,  -8191,-8192,  -8191,  -8190,  -8186,  -8182,  -8177,  -8170,  -8162,  -8153,  -8142,  -8130,  -8117,  -8103,  -8088,  -8071,  -8054,-8035,  -8014,  -7993,  -7970,  -7946,  -7921,  -7895,  -7868,  -7839,  -7809,  -7779,  -7746,  -7713,  -7679,  -7643,  -7606,-7568,  -7529,  -7489,  -7448,  -7405,  -7362,  -7317,  -7272,  -7225,  -7177,  -7128,  -7078,  -7027,  -6974,  -6921,  -6867,-6811,  -6755,  -6698,  -6639,  -6580,  -6519,  -6458,  -6396,  -6333,  -6268,  -6203,  -6137,  -6070,  -6002,  -5933,  -5863,-5793,  -5721,  -5649,  -5575,  -5501,  -5427,  -5351,  -5274,  -5197,  -5119,  -5040,  -4960,  -4880,  -4799,  -4717,  -4634,-4551,  -4467,  -4383,  -4297,  -4212,  -4125,  -4038,  -3950,  -3862,  -3773,  -3683,  -3593,  -3503,  -3411,  -3320,  -3228,-3135,  -3042,  -2948,  -2854,  -2760,  -2665,  -2570,  -2474,  -2378,  -2282,  -2185,  -2088,  -1990,  -1893,  -1795,  -1697,-1598,  -1499,  -1401,  -1301,  -1202,  -1102,  -1003,   -903,   -803,   -703,   -603,   -502,   -402,   -302,   -201,   -101,+0,};
    ///////////////////////////////////////////////////////////////
    
    int16_t triangle[2048] = {0, -32, -64, -96, -128, -160, -192, -224, -256, -288, -320, -352, -384, -416, -448, -480, -512, -544, -576, -608, -640, -672, -704, -736, -768, -800, -832, -864, -896, -928, -960, -992, -1024, -1056, -1088, -1120, -1152, -1184, -1216, -1248, -1280, -1312, -1344, -1376, -1408, -1440, -1472, -1504, -1536, -1568, -1600, -1632, -1664, -1696, -1728, -1760, -1792, -1824, -1856, -1888, -1920, -1952, -1984, -2016, -2048, -2080, -2112, -2144, -2176, -2208, -2240, -2272, -2304, -2336, -2368, -2400, -2432, -2464, -2496, -2528, -2560, -2592, -2624, -2656, -2688, -2720, -2752, -2784, -2816, -2848, -2880, -2912, -2944, -2976, -3008, -3040, -3072, -3104, -3136, -3168, -3200, -3232, -3264, -3296, -3328, -3360, -3392, -3424, -3456, -3488, -3520, -3552, -3584, -3616, -3648, -3680, -3712, -3744, -3776, -3808, -3840, -3872, -3904, -3936, -3968, -4000, -4032, -4064, -4096, -4128, -4160, -4192, -4224, -4256, -4288, -4320, -4352, -4384, -4416, -4448, -4480, -4512, -4544, -4576, -4608, -4640, -4672, -4704, -4736, -4768, -4800, -4832, -4864, -4896, -4928, -4960, -4992, -5024, -5056, -5088, -5120, -5152, -5184, -5216, -5248, -5280, -5312, -5344, -5376, -5408, -5440, -5472, -5504, -5536, -5568, -5600, -5632, -5664, -5696, -5728, -5760, -5792, -5824, -5856, -5888, -5920, -5952, -5984, -6016, -6048, -6080, -6112, -6144, -6176, -6208, -6240, -6272, -6304, -6336, -6368, -6400, -6432, -6464, -6496, -6528, -6560, -6592, -6624, -6656, -6688, -6720, -6752, -6784, -6816, -6848, -6880, -6912, -6944, -6976, -7008, -7040, -7072, -7104, -7136, -7168, -7200, -7232, -7264, -7296, -7328, -7360, -7392, -7424, -7456, -7488, -7520, -7552, -7584, -7616, -7648, -7680, -7712, -7744, -7776, -7808, -7840, -7872, -7904, -7936, -7968, -8000, -8032, -8064, -8096, -8128, -8160, -8192, -8224, -8256, -8288, -8320, -8352, -8384, -8416, -8448, -8480, -8512, -8544, -8576, -8608, -8640, -8672, -8704, -8736, -8768, -8800, -8832, -8864, -8896, -8928, -8960, -8992, -9024, -9056, -9088, -9120, -9152, -9184, -9216, -9248, -9280, -9312, -9344, -9376, -9408, -9440, -9472, -9504, -9536, -9568, -9600, -9632, -9664, -9696, -9728, -9760, -9792, -9824, -9856, -9888, -9920, -9952, -9984, -10016, -10048, -10080, -10112, -10144, -10176, -10208, -10240, -10272, -10304, -10336, -10368, -10400, -10432, -10464, -10496, -10528, -10560, -10592, -10624, -10656, -10688, -10720, -10752, -10784, -10816, -10848, -10880, -10912, -10944, -10976, -11008, -11040, -11072, -11104, -11136, -11168, -11200, -11232, -11264, -11296, -11328, -11360, -11392, -11424, -11456, -11488, -11520, -11552, -11584, -11616, -11648, -11680, -11712, -11744, -11776, -11808, -11840, -11872, -11904, -11936, -11968, -12000, -12032, -12064, -12096, -12128, -12160, -12192, -12224, -12256, -12288, -12320, -12352, -12384, -12416, -12448, -12480, -12512, -12544, -12576, -12608, -12640, -12672, -12704, -12736, -12768, -12800, -12832, -12864, -12896, -12928, -12960, -12992, -13024, -13056, -13088, -13120, -13152, -13184, -13216, -13248, -13280, -13312, -13344, -13376, -13408, -13440, -13472, -13504, -13536, -13568, -13600, -13632, -13664, -13696, -13728, -13760, -13792, -13824, -13856, -13888, -13920, -13952, -13984, -14016, -14048, -14080, -14112, -14144, -14176, -14208, -14240, -14272, -14304, -14336, -14368, -14400, -14432, -14464, -14496, -14528, -14560, -14592, -14624, -14656, -14688, -14720, -14752, -14784, -14816, -14848, -14880, -14912, -14944, -14976, -15008, -15040, -15072, -15104, -15136, -15168, -15200, -15232, -15264, -15296, -15328, -15360, -15392, -15424, -15456, -15488, -15520, -15552, -15584, -15616, -15648, -15680, -15712, -15744, -15776, -15808, -15840, -15872, -15904, -15936, -15968, -16000, -16032, -16064, -16096, -16128, -16160, -16192, -16224, -16256, -16288, -16320, -16352, -16384, -16352, -16320, -16288, -16256, -16224, -16192, -16160, -16128, -16096, -16064, -16032, -16000, -15968, -15936, -15904, -15872, -15840, -15808, -15776, -15744, -15712, -15680, -15648, -15616, -15584, -15552, -15520, -15488, -15456, -15424, -15392, -15360, -15328, -15296, -15264, -15232, -15200, -15168, -15136, -15104, -15072, -15040, -15008, -14976, -14944, -14912, -14880, -14848, -14816, -14784, -14752, -14720, -14688, -14656, -14624, -14592, -14560, -14528, -14496, -14464, -14432, -14400, -14368, -14336, -14304, -14272, -14240, -14208, -14176, -14144, -14112, -14080, -14048, -14016, -13984, -13952, -13920, -13888, -13856, -13824, -13792, -13760, -13728, -13696, -13664, -13632, -13600, -13568, -13536, -13504, -13472, -13440, -13408, -13376, -13344, -13312, -13280, -13248, -13216, -13184, -13152, -13120, -13088, -13056, -13024, -12992, -12960, -12928, -12896, -12864, -12832, -12800, -12768, -12736, -12704, -12672, -12640, -12608, -12576, -12544, -12512, -12480, -12448, -12416, -12384, -12352, -12320, -12288, -12256, -12224, -12192, -12160, -12128, -12096, -12064, -12032, -12000, -11968, -11936, -11904, -11872, -11840, -11808, -11776, -11744, -11712, -11680, -11648, -11616, -11584, -11552, -11520, -11488, -11456, -11424, -11392, -11360, -11328, -11296, -11264, -11232, -11200, -11168, -11136, -11104, -11072, -11040, -11008, -10976, -10944, -10912, -10880, -10848, -10816, -10784, -10752, -10720, -10688, -10656, -10624, -10592, -10560, -10528, -10496, -10464, -10432, -10400, -10368, -10336, -10304, -10272, -10240, -10208, -10176, -10144, -10112, -10080, -10048, -10016, -9984, -9952, -9920, -9888, -9856, -9824, -9792, -9760, -9728, -9696, -9664, -9632, -9600, -9568, -9536, -9504, -9472, -9440, -9408, -9376, -9344, -9312, -9280, -9248, -9216, -9184, -9152, -9120, -9088, -9056, -9024, -8992, -8960, -8928, -8896, -8864, -8832, -8800, -8768, -8736, -8704, -8672, -8640, -8608, -8576, -8544, -8512, -8480, -8448, -8416, -8384, -8352, -8320, -8288, -8256, -8224, -8192, -8160, -8128, -8096, -8064, -8032, -8000, -7968, -7936, -7904, -7872, -7840, -7808, -7776, -7744, -7712, -7680, -7648, -7616, -7584, -7552, -7520, -7488, -7456, -7424, -7392, -7360, -7328, -7296, -7264, -7232, -7200, -7168, -7136, -7104, -7072, -7040, -7008, -6976, -6944, -6912, -6880, -6848, -6816, -6784, -6752, -6720, -6688, -6656, -6624, -6592, -6560, -6528, -6496, -6464, -6432, -6400, -6368, -6336, -6304, -6272, -6240, -6208, -6176, -6144, -6112, -6080, -6048, -6016, -5984, -5952, -5920, -5888, -5856, -5824, -5792, -5760, -5728, -5696, -5664, -5632, -5600, -5568, -5536, -5504, -5472, -5440, -5408, -5376, -5344, -5312, -5280, -5248, -5216, -5184, -5152, -5120, -5088, -5056, -5024, -4992, -4960, -4928, -4896, -4864, -4832, -4800, -4768, -4736, -4704, -4672, -4640, -4608, -4576, -4544, -4512, -4480, -4448, -4416, -4384, -4352, -4320, -4288, -4256, -4224, -4192, -4160, -4128, -4096, -4064, -4032, -4000, -3968, -3936, -3904, -3872, -3840, -3808, -3776, -3744, -3712, -3680, -3648, -3616, -3584, -3552, -3520, -3488, -3456, -3424, -3392, -3360, -3328, -3296, -3264, -3232, -3200, -3168, -3136, -3104, -3072, -3040, -3008, -2976, -2944, -2912, -2880, -2848, -2816, -2784, -2752, -2720, -2688, -2656, -2624, -2592, -2560, -2528, -2496, -2464, -2432, -2400, -2368, -2336, -2304, -2272, -2240, -2208, -2176, -2144, -2112, -2080, -2048, -2016, -1984, -1952, -1920, -1888, -1856, -1824, -1792, -1760, -1728, -1696, -1664, -1632, -1600, -1568, -1536, -1504, -1472, -1440, -1408, -1376, -1344, -1312, -1280, -1248, -1216, -1184, -1152, -1120, -1088, -1056, -1024, -992, -960, -928, -896, -864, -832, -800, -768, -736, -704, -672, -640, -608, -576, -544, -512, -480, -448, -416, -384, -352, -320, -288, -256, -224, -192, -160, -128, -96, -64, -32, 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672, 704, 736, 768, 800, 832, 864, 896, 928, 960, 992, 1024, 1056, 1088, 1120, 1152, 1184, 1216, 1248, 1280, 1312, 1344, 1376, 1408, 1440, 1472, 1504, 1536, 1568, 1600, 1632, 1664, 1696, 1728, 1760, 1792, 1824, 1856, 1888, 1920, 1952, 1984, 2016, 2048, 2080, 2112, 2144, 2176, 2208, 2240, 2272, 2304, 2336, 2368, 2400, 2432, 2464, 2496, 2528, 2560, 2592, 2624, 2656, 2688, 2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944, 2976, 3008, 3040, 3072, 3104, 3136, 3168, 3200, 3232, 3264, 3296, 3328, 3360, 3392, 3424, 3456, 3488, 3520, 3552, 3584, 3616, 3648, 3680, 3712, 3744, 3776, 3808, 3840, 3872, 3904, 3936, 3968, 4000, 4032, 4064, 4096, 4128, 4160, 4192, 4224, 4256, 4288, 4320, 4352, 4384, 4416, 4448, 4480, 4512, 4544, 4576, 4608, 4640, 4672, 4704, 4736, 4768, 4800, 4832, 4864, 4896, 4928, 4960, 4992, 5024, 5056, 5088, 5120, 5152, 5184, 5216, 5248, 5280, 5312, 5344, 5376, 5408, 5440, 5472, 5504, 5536, 5568, 5600, 5632, 5664, 5696, 5728, 5760, 5792, 5824, 5856, 5888, 5920, 5952, 5984, 6016, 6048, 6080, 6112, 6144, 6176, 6208, 6240, 6272, 6304, 6336, 6368, 6400, 6432, 6464, 6496, 6528, 6560, 6592, 6624, 6656, 6688, 6720, 6752, 6784, 6816, 6848, 6880, 6912, 6944, 6976, 7008, 7040, 7072, 7104, 7136, 7168, 7200, 7232, 7264, 7296, 7328, 7360, 7392, 7424, 7456, 7488, 7520, 7552, 7584, 7616, 7648, 7680, 7712, 7744, 7776, 7808, 7840, 7872, 7904, 7936, 7968, 8000, 8032, 8064, 8096, 8128, 8160, 8192, 8224, 8256, 8288, 8320, 8352, 8384, 8416, 8448, 8480, 8512, 8544, 8576, 8608, 8640, 8672, 8704, 8736, 8768, 8800, 8832, 8864, 8896, 8928, 8960, 8992, 9024, 9056, 9088, 9120, 9152, 9184, 9216, 9248, 9280, 9312, 9344, 9376, 9408, 9440, 9472, 9504, 9536, 9568, 9600, 9632, 9664, 9696, 9728, 9760, 9792, 9824, 9856, 9888, 9920, 9952, 9984, 10016, 10048, 10080, 10112, 10144, 10176, 10208, 10240, 10272, 10304, 10336, 10368, 10400, 10432, 10464, 10496, 10528, 10560, 10592, 10624, 10656, 10688, 10720, 10752, 10784, 10816, 10848, 10880, 10912, 10944, 10976, 11008, 11040, 11072, 11104, 11136, 11168, 11200, 11232, 11264, 11296, 11328, 11360, 11392, 11424, 11456, 11488, 11520, 11552, 11584, 11616, 11648, 11680, 11712, 11744, 11776, 11808, 11840, 11872, 11904, 11936, 11968, 12000, 12032, 12064, 12096, 12128, 12160, 12192, 12224, 12256, 12288, 12320, 12352, 12384, 12416, 12448, 12480, 12512, 12544, 12576, 12608, 12640, 12672, 12704, 12736, 12768, 12800, 12832, 12864, 12896, 12928, 12960, 12992, 13024, 13056, 13088, 13120, 13152, 13184, 13216, 13248, 13280, 13312, 13344, 13376, 13408, 13440, 13472, 13504, 13536, 13568, 13600, 13632, 13664, 13696, 13728, 13760, 13792, 13824, 13856, 13888, 13920, 13952, 13984, 14016, 14048, 14080, 14112, 14144, 14176, 14208, 14240, 14272, 14304, 14336, 14368, 14400, 14432, 14464, 14496, 14528, 14560, 14592, 14624, 14656, 14688, 14720, 14752, 14784, 14816, 14848, 14880, 14912, 14944, 14976, 15008, 15040, 15072, 15104, 15136, 15168, 15200, 15232, 15264, 15296, 15328, 15360, 15392, 15424, 15456, 15488, 15520, 15552, 15584, 15616, 15648, 15680, 15712, 15744, 15776, 15808, 15840, 15872, 15904, 15936, 15968, 16000, 16032, 16064, 16096, 16128, 16160, 16192, 16224, 16256, 16288, 16320, 16352, 16384, 16352, 16320, 16288, 16256, 16224, 16192, 16160, 16128, 16096, 16064, 16032, 16000, 15968, 15936, 15904, 15872, 15840, 15808, 15776, 15744, 15712, 15680, 15648, 15616, 15584, 15552, 15520, 15488, 15456, 15424, 15392, 15360, 15328, 15296, 15264, 15232, 15200, 15168, 15136, 15104, 15072, 15040, 15008, 14976, 14944, 14912, 14880, 14848, 14816, 14784, 14752, 14720, 14688, 14656, 14624, 14592, 14560, 14528, 14496, 14464, 14432, 14400, 14368, 14336, 14304, 14272, 14240, 14208, 14176, 14144, 14112, 14080, 14048, 14016, 13984, 13952, 13920, 13888, 13856, 13824, 13792, 13760, 13728, 13696, 13664, 13632, 13600, 13568, 13536, 13504, 13472, 13440, 13408, 13376, 13344, 13312, 13280, 13248, 13216, 13184, 13152, 13120, 13088, 13056, 13024, 12992, 12960, 12928, 12896, 12864, 12832, 12800, 12768, 12736, 12704, 12672, 12640, 12608, 12576, 12544, 12512, 12480, 12448, 12416, 12384, 12352, 12320, 12288, 12256, 12224, 12192, 12160, 12128, 12096, 12064, 12032, 12000, 11968, 11936, 11904, 11872, 11840, 11808, 11776, 11744, 11712, 11680, 11648, 11616, 11584, 11552, 11520, 11488, 11456, 11424, 11392, 11360, 11328, 11296, 11264, 11232, 11200, 11168, 11136, 11104, 11072, 11040, 11008, 10976, 10944, 10912, 10880, 10848, 10816, 10784, 10752, 10720, 10688, 10656, 10624, 10592, 10560, 10528, 10496, 10464, 10432, 10400, 10368, 10336, 10304, 10272, 10240, 10208, 10176, 10144, 10112, 10080, 10048, 10016, 9984, 9952, 9920, 9888, 9856, 9824, 9792, 9760, 9728, 9696, 9664, 9632, 9600, 9568, 9536, 9504, 9472, 9440, 9408, 9376, 9344, 9312, 9280, 9248, 9216, 9184, 9152, 9120, 9088, 9056, 9024, 8992, 8960, 8928, 8896, 8864, 8832, 8800, 8768, 8736, 8704, 8672, 8640, 8608, 8576, 8544, 8512, 8480, 8448, 8416, 8384, 8352, 8320, 8288, 8256, 8224, 8192, 8160, 8128, 8096, 8064, 8032, 8000, 7968, 7936, 7904, 7872, 7840, 7808, 7776, 7744, 7712, 7680, 7648, 7616, 7584, 7552, 7520, 7488, 7456, 7424, 7392, 7360, 7328, 7296, 7264, 7232, 7200, 7168, 7136, 7104, 7072, 7040, 7008, 6976, 6944, 6912, 6880, 6848, 6816, 6784, 6752, 6720, 6688, 6656, 6624, 6592, 6560, 6528, 6496, 6464, 6432, 6400, 6368, 6336, 6304, 6272, 6240, 6208, 6176, 6144, 6112, 6080, 6048, 6016, 5984, 5952, 5920, 5888, 5856, 5824, 5792, 5760, 5728, 5696, 5664, 5632, 5600, 5568, 5536, 5504, 5472, 5440, 5408, 5376, 5344, 5312, 5280, 5248, 5216, 5184, 5152, 5120, 5088, 5056, 5024, 4992, 4960, 4928, 4896, 4864, 4832, 4800, 4768, 4736, 4704, 4672, 4640, 4608, 4576, 4544, 4512, 4480, 4448, 4416, 4384, 4352, 4320, 4288, 4256, 4224, 4192, 4160, 4128, 4096, 4064, 4032, 4000, 3968, 3936, 3904, 3872, 3840, 3808, 3776, 3744, 3712, 3680, 3648, 3616, 3584, 3552, 3520, 3488, 3456, 3424, 3392, 3360, 3328, 3296, 3264, 3232, 3200, 3168, 3136, 3104, 3072, 3040, 3008, 2976, 2944, 2912, 2880, 2848, 2816, 2784, 2752, 2720, 2688, 2656, 2624, 2592, 2560, 2528, 2496, 2464, 2432, 2400, 2368, 2336, 2304, 2272, 2240, 2208, 2176, 2144, 2112, 2080, 2048, 2016, 1984, 1952, 1920, 1888, 1856, 1824, 1792, 1760, 1728, 1696, 1664, 1632, 1600, 1568, 1536, 1504, 1472, 1440, 1408, 1376, 1344, 1312, 1280, 1248, 1216, 1184, 1152, 1120, 1088, 1056, 1024, 992, 960, 928, 896, 864, 832, 800, 768, 736, 704, 672, 640, 608, 576, 544, 512, 480, 448, 416, 384, 352, 320, 288, 256, 224, 192, 160, 128, 96, 64, 32};

    uint8_t bitShift(size_t tableSize) {
        uint8_t shift = 0;
        while (tableSize > 1) {
            tableSize >>= 1;
            shift++;
        }
        return 32 - shift;
    }

    void setFrequency(int noteIndex, float frequency) {
        if (noteIndex >= 0 && noteIndex < MAX_NOTES) {
            notes[noteIndex].phase_delta = frequency * (float)(1ULL << 32) / sample_rate;
        }
    }

    float midiNoteToFrequency(uint8_t midiNote) {
        return 440.0 * pow(2.0, (midiNote - 69) / 12.0);
    }

    int8_t getOldNote() {
        int8_t index = -1;
        uint8_t min = 0xff;
        for(uint8_t i = 0; i < MAX_NOTES; i++) {
            if(getActiveNote() == MAX_NOTES) {
                if(notes[i].actnum < min){
                    min = notes[i].actnum;
                    index = i;
                }
            } else if(notes[i].active == false) {
                index = i;
            }
        }
        return index;
    }

    int8_t getNoteIndex(uint8_t note) {
        int8_t index = -1;
        for(uint8_t i = 0; i < MAX_NOTES; i++) {
            if(notes[i].note == note) index = i;
        }
        return index;
    }

    void updateActNum(int noteIndex) {
        if (noteIndex < 0 || noteIndex >= MAX_NOTES) {
            return;
        }
        if (!notes[noteIndex].active) {
            return;
        }
        for (int i = 0; i < MAX_NOTES; ++i) {
            // ノートがアクティブであり、かつそのactnumが
            // 更新されたノートのactnumより大きい場合、デクリメントする
            if (notes[i].active && notes[i].actnum > notes[noteIndex].actnum) {
                notes[i].actnum--;
            }
        }
    }


    bool isActiveNote(uint8_t _note) {
        bool active = false;
        for(Note note: notes) {
            if(note.note == _note && note.active == true){
                active = true;
            }
        }
        return active;
    }

public:
    WaveGenerator(int32_t rate, float gain = 1.0f): volume_gain(gain), sample_rate(rate) {
        for (int i = 0; i < MAX_NOTES; ++i) {
            notes[i].active = false;
            notes[i].phase = 0;
            notes[i].phase_delta = 0;
            notes[i].actnum = 0;
            notes[i].note = 0xff;
        }
    }

    uint8_t getActiveNote() {
        uint8_t active = 0;
        for(Note note: notes) {
            if(note.active == true) active++;
        }
        return active;
    }

    void noteOn(uint8_t note) {
        if(isActiveNote(note)) return;
        if(note > 127) return;

        int8_t noteIndex = getOldNote();
        if(noteIndex == -1) return;
        setFrequency(noteIndex, midiNoteToFrequency(note));
        notes[noteIndex].note = note;
        notes[noteIndex].actnum = getActiveNote();
        notes[noteIndex].active = true;
    }

    void noteOff(uint8_t note) {
        if(!isActiveNote(note)) return;

        int8_t noteIndex = getNoteIndex(note);
        if(noteIndex == -1) return;
        notes[noteIndex].phase = 0;
        notes[noteIndex].phase_delta = 0;
        notes[noteIndex].actnum = 0;
        notes[noteIndex].note = 0xff;
        notes[noteIndex].active = false;
        updateActNum(noteIndex);
    }

    void noteReset() {
        for(Note note: notes) {
            note.phase = 0;
            note.phase_delta = 0;
            note.active = false;
            note.actnum = 0;
            note.note = 0xff;
        }
    }

    void generate(int16_t *buffer, size_t size) {
        memset(buffer, 0, sizeof(int16_t) * size); // バッファをクリア

        for (int n = 0; n < MAX_NOTES; ++n) {
            if (notes[n].active) {
                size_t sampleSize;
                int16_t* waveform;

                switch(preset) {
                    case 0x00:
                        sampleSize = sizeof(sine) / sizeof(sine[0]);
                        waveform = sine;
                        break;
                    case 0x01:
                        sampleSize = sizeof(square) / sizeof(square[0]);
                        waveform = square;
                        break;
                    case 0x02:
                        sampleSize = sizeof(saw) / sizeof(saw[0]);
                        waveform = saw;
                        break;
                    case 0x03:
                        sampleSize = sizeof(triangle) / sizeof(triangle[0]);
                        waveform = triangle;
                        break;
                }

                if (waveform != nullptr) {
                    for (size_t i = 0; i < size; i++) {
                        int16_t value = waveform[(notes[n].phase >> bitShift(sampleSize)) % sampleSize];
                        buffer[i] += constrain(value * (volume_gain / MAX_NOTES), INT16_MIN, INT16_MAX);
                        notes[n].phase += notes[n].phase_delta;
                    }
                }
            }
        }

        // 必要に応じてバッファの正規化
        for (size_t i = 0; i < size; i++) {
            buffer[i] = constrain(buffer[i], INT16_MIN, INT16_MAX);
        }
    }

    void setPreset(uint8_t id) {
        preset = id;
    }
};