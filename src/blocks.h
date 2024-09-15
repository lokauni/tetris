
const int BLOCK_TYPES[2][4][4][4] = {{
                                         {
                                             {1, 1, 0, 0},
                                             {1, 0, 0, 0},
                                             {1, 0, 0, 0},
                                             {0, 0, 0, 0},
                                         },
                                         {
                                             {1, 1, 1, 0},
                                             {0, 0, 1, 0},
                                             {0, 0, 0, 0},
                                             {0, 0, 0, 0},
                                         },
                                         {
                                             {0, 1, 0, 0},
                                             {0, 1, 0, 0},
                                             {1, 1, 0, 0},
                                             {0, 0, 0, 0},
                                         },
                                         {
                                             {1, 0, 0, 0},
                                             {1, 1, 1, 0},
                                             {0, 0, 0, 0},
                                             {0, 0, 0, 0},
                                         },
                                     },
                                     {
                                         {
                                             {1, 0, 0, 0},
                                             {1, 0, 0, 0},
                                             {1, 0, 0, 0},
                                             {1, 0, 0, 0},
                                         },
                                         {
                                             {0, 0, 0, 0},
                                             {1, 1, 1, 1},
                                             {0, 0, 0, 0},
                                             {0, 0, 0, 0},
                                         },
                                         {
                                             {1, 0, 0, 0},
                                             {1, 0, 0, 0},
                                             {1, 0, 0, 0},
                                             {1, 0, 0, 0},
                                         },
                                         {
                                             {0, 0, 0, 0},
                                             {1, 1, 1, 1},
                                             {0, 0, 0, 0},
                                             {0, 0, 0, 0},
                                         },
                                     }};

typedef struct {
    int type;
    int rotation;
    int x;
    int y;
} Block;

int block_get_width(int type, int rotation) {
    for (int i = 3; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            int cell = BLOCK_TYPES[type][rotation][j][i];
            if (cell) {
                return i + 1;
            }
        }
    }
    return 0;
}

int block_get_height(int type, int rotation) {
    for (int j = 3; j >= 0; j--) {
        for (int i = 0; i < 4; i++) {
            int cell = BLOCK_TYPES[type][rotation][j][i];
            if (cell) {
                return j + 1;
            }
        }
    }
    return 0;
}

