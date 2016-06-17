from enum import IntEnum, unique

@unique
class CUBA_EXT(IntEnum):
    """CUBA extension words that are not officialy approved yet."""
    SYMMETRY_GROUP = 8
    SHAPE_TYPE = 16
    LATTICE_UC_ABC = 68
    LATTICE_UC_ANGLES = 69
    CRYSTAL_ORIENTATION_1 = 70
    CRYSTAL_ORIENTATION_2 = 71
    SHAPE_ORIENTATION_1 = 72
    SHAPE_ORIENTATION_2 = 73
    FILE_STL = 74
    STL_MODE = 75
    STL_SCALING = 76
    STL_X_NEG_PADDING = 77
    STL_X_POS_PADDING = 78
    STL_Y_NEG_PADDING = 79
    STL_Y_POS_PADDING = 80
    STL_Z_NEG_PADDING = 81
    STL_Z_POS_PADDING = 82

@unique
class SHAPE_TYPE(IntEnum):

    DIM_3D_BLOCK_UC    = 1
    DIM_3D_BLOCK_XYZ   = 2
    DIM_3D_SPHERE      = 3 
    DIM_3D_CYLINDER    = 4 
    DIM_3D_HEXPRISM    = 5
    DIM_2D_BLOCK_UC    = 6 
    DIM_2D_BLOCK_XYZ   = 7
    DIM_2D_DISK        = 8 
    DIM_2D_HEXAGON     = 9
    DIM_1D_BLOCK_UC    = 10 
    DIM_1D_BLOCK_XYZ   = 11
    DIM_0D_ATOM_LIST   = 12
    DIM_3D_STL         = 13

@unique
class AXIS_TYPE(IntEnum):

    X   = 1
    Y   = 2
    Z   = 3
    
@unique
class SYMMETRY_GROUP(IntEnum):

    P1          = 1
    P_1         = 2
    P121        = 3
    P1211       = 4
    C121        = 5
    P1M1        = 6
    P1C1        = 7
    C1M1        = 8
    C1C1        = 9
    P12_M1      = 10
    P121_M1     = 11
    C12_M1      = 12
    P12_C1      = 13
    P121_C1     = 14
    C12_C1      = 15
    P222        = 16
    P2221       = 17
    P21212      = 18
    P212121     = 19
    C2221       = 20
    C222        = 21
    F222        = 22
    I222        = 23
    I212121     = 24
    PMM2        = 25
    PMC21       = 26
    PCC2        = 27
    PMA2        = 28
    PCA21       = 29
    PNC2        = 30
    PMN21       = 31
    PBA2        = 32
    PNA21       = 33
    PNN2        = 34
    CMM2        = 35
    CMC21       = 36
    CCC2        = 37
    AMM2        = 38
    ABM2        = 39
    AMA2        = 40
    ABA2        = 41
    FMM2        = 42
    FDD2        = 43
    IMM2        = 44
    IBA2        = 45
    IMA2        = 46
    PMMM        = 47
    PNNN_1      = 48
    PCCM        = 49
    PBAN_1      = 50
    PMMA        = 51
    PNNA        = 52
    PMNA        = 53
    PCCA        = 54
    PBAM        = 55
    PCCN        = 56
    PBCM        = 57
    PNNM        = 58
    PMMN_1      = 59
    PBCN        = 60
    PBCA        = 61
    PNMA        = 62
    CMCM        = 63
    CMCA        = 64
    CMMM        = 65
    CCCM        = 66
    CMMA        = 67
    CCCA_1      = 68
    FMMM        = 69
    FDDD_1      = 70
    IMMM        = 71
    IBAM        = 72
    IBCA        = 73
    IMMA        = 74
    P4          = 75
    P41         = 76
    P42         = 77
    P43         = 78
    I4          = 79
    I41         = 80
    P_4         = 81
    I_4         = 82
    P4_M        = 83
    P42_M       = 84
    P4_N_1      = 85
    P42_N_1     = 86
    I4_M        = 87
    I41_A_1     = 88
    P422        = 89
    P4212       = 90
    P4122       = 91
    P41212      = 92
    P4222       = 93
    P42212      = 94
    P4322       = 95
    P43212      = 96
    I422        = 97
    I4122       = 98
    P4MM        = 99
    P4BM        = 100
    P42CM       = 101
    P42NM       = 102
    P4CC        = 103
    P4NC        = 104
    P42MC       = 105
    P42BC       = 106
    I4MM        = 107
    I4CM        = 108
    I41MD       = 109
    I41CD       = 110
    P_42M       = 111
    P_42C       = 112
    P_421M      = 113
    P_421C      = 114
    P_4M2       = 115
    P_4C2       = 116
    P_4B2       = 117
    P_4N2       = 118
    I_4M2       = 119
    I_4C2       = 120
    I_42M       = 121
    I_42D       = 122
    P4_MMM      = 123
    P4_MCC      = 124
    P4_NBM_1    = 125
    P4_NNC_1    = 126
    P4_MBM      = 127
    P4_MNC      = 128
    P4_NMM_1    = 129
    P4_NCC_1    = 130
    P42_MMC     = 131
    P42_MCM     = 132
    P42_NBC_1   = 133
    P42_NNM_1   = 134
    P42_MBC     = 135
    P42_MNM     = 136
    P42_NMC_1   = 137
    P42_NCM_1   = 138
    I4_MMM      = 139
    I4_MCM      = 140
    I41_AMD_1   = 141
    I41_ACD_1   = 142
    P3          = 143
    P31         = 144
    P32         = 145
    R3_H        = 146
    P_3         = 147
    R_3_H       = 148
    P312        = 149
    P321        = 150
    P3112       = 151
    P3121       = 152
    P3212       = 153
    P3221       = 154
    R32_H       = 155  
    P3M1        = 156
    P31M        = 157
    P3C1        = 158  
    P31C        = 159  
    R3M_H       = 160  
    R3C_H       = 161  
    P_31M       = 162  
    P_31C       = 163  
    P_3M1       = 164
    P_3C1       = 165       
    R_3M_H      = 166       
    R_3C_H      = 167       
    P6          = 168       
    P61         = 169       
    P65         = 170       
    P62         = 171       
    P64         = 172       
    P63         = 173       
    P_6         = 174       
    P6_M        = 175       
    P63_M       = 176       
    P622        = 177       
    P6122       = 178       
    P6522       = 179       
    P6222       = 180       
    P6422       = 181       
    P6322       = 182       
    P6MM        = 183       
    P6CC        = 184       
    P63CM       = 185       
    P63MC       = 186       
    P_6M2       = 187       
    P_6C2       = 188       
    P_62M       = 189       
    P_62C       = 190       
    P6_MMM      = 191       
    P6_MCC      = 192       
    P63_MCM     = 193       
    P63_MMC     = 194       
    P23         = 195       
    F23         = 196       
    I23         = 197       
    P213        = 198       
    I213        = 199       
    PM_3        = 200       
    PN_3_1      = 201       
    FM_3        = 202       
    FD_3_1      = 203       
    IM_3        = 204       
    PA_3        = 205       
    IA_3        = 206       
    P432        = 207       
    P4232       = 208       
    F432        = 209       
    F4132       = 210       
    I432        = 211       
    P4332       = 212       
    P4132       = 213       
    I4132       = 214       
    P_43M       = 215       
    F_43M       = 216       
    I_43M       = 217       
    P_43N       = 218       
    F_43C       = 219       
    I_43D       = 220       
    PM_3M       = 221       
    PN_3N_1     = 222       
    PM_3N       = 223       
    PN_3M_1     = 224       
    FM_3M       = 225       
    FM_3C       = 226       
    FD_3M_1     = 227       
    FD_3C_1     = 228       
    IM_3M       = 229       
    IA_3D       = 230       
