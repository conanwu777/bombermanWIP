
typedef struct __attribute__ ((packed)) noise_config
{
    int octaves;
    float frequency;
    float lacunarity;
    float amplitude;
    float persistence;
} noise_config;

int fastfloor(float const x);
float scaled_raw_noise_3d(__constant int *prm, float const loBound, float const hiBound, float const x, float const y, float const z);
float raw_noise_3d(__constant int *prm, float const x, float const y, float const z);
float octave_noise_3d(__constant int *prm, __constant noise_config *nc,
                      float const x, float const y, float const z);

#define CHUNK_SIZE 32
#define WATER_HEIGHT 90.5
#define BEACH_MAX_HEIGHT 5

typedef enum
{
    AIR,
    SIDE_GRASS,
    DIRT,
    GRASS,
    STONE,
    COAL,
    LEAF,
    WOOD,
    SAND,
    SAND_SIDE,
    WATER,
    WEED,
    TREE_SEED,
} block_type;

float computeMountainCaveNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return (octave_noise_3d(prm, &noise_configs[0], *wx, *wy, *wz)
          + octave_noise_3d(prm, &noise_configs[1], *wx, *wy, *wz)) / 2.0f;
}

float computeMountainNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return (octave_noise_3d(prm, &noise_configs[2], *wx, *wy, *wz) * 3.0f
          + octave_noise_3d(prm, &noise_configs[3], *wx, *wy, *wz) * 3.0f
          + octave_noise_3d(prm, &noise_configs[4], *wx, *wy, *wz) * 3.0f
          + octave_noise_3d(prm, &noise_configs[5], *wx, *wy, *wz)) / 4.0f;
}

float computeValleyCaveNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return (octave_noise_3d(prm, &noise_configs[0], *wx, *wy, *wz)
          + octave_noise_3d(prm, &noise_configs[1], *wx, *wy, *wz)) / 2.0f;
}

float computeValleyNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return octave_noise_3d(prm, &noise_configs[7], *wx, *wy, *wz) * 2.5f;
}

float computeVolcanoCaveNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return octave_noise_3d(prm, &noise_configs[1], *wx, *wy, *wz);
}

float computeVolcanoNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return octave_noise_3d(prm, &noise_configs[4], *wx, *wy, *wz);
}

float computeBiomeNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return octave_noise_3d(prm, &noise_configs[6], *wx, 0.0f, *wz);
}

float computeTreeNoise(__constant int *prm, __constant noise_config *noise_configs, float const *wx, float const *wy, float const *wz)
{
    return octave_noise_3d(prm, &noise_configs[8], *wx, 0.0f, *wz);
}

__kernel void generateBlock(__global char *chunk, __constant noise_config *noise_configs, __constant int *prm,
                            float chunk_x, float chunk_y, float chunk_z)
{
    int const i = get_global_id(0);

    int idx = i;
    int x = idx / (CHUNK_SIZE * CHUNK_SIZE);
    idx -= (x * CHUNK_SIZE * CHUNK_SIZE);
    int y = idx / CHUNK_SIZE;
    int z = idx % CHUNK_SIZE;

    float const wx = (chunk_x + x);
    float const wy = (chunk_y + y);
    float const wz = (chunk_z + z);

    /// Biome
    float biome_noise = computeBiomeNoise(prm, noise_configs, &wx, &wy, &wz);

    // 1.0f -> valley, 0.0f -> volcano
    float st1 = smoothstep(0.28f, 0.38f, biome_noise);
    // 1.0f -> mountain, 0.0f -> valley
    float st2 = smoothstep(0.56f, 0.66f, biome_noise);

    // si st1 == 1.0f et st2 == 1.0f -> mountain!
    // si st1 == 0.0f et st2 == 0.0f -> volcano!
    // si st1 == 1.0f et st2 == 0.0f -> valley!

    float noise = 0.0f;
    float cave_noise = 0.0f;
    // transition entre volcano et valley
    if (st1 > 0.01f && st1 < 0.99f)
    {
        noise = (st1 * computeValleyNoise(prm, noise_configs, &wx, &wy, &wz) + (1.0f - st1) * computeVolcanoNoise(prm, noise_configs, &wx, &wy, &wz));
        cave_noise = (st1 * computeValleyCaveNoise(prm, noise_configs, &wx, &wy, &wz) + (1.0f - st1) * computeVolcanoCaveNoise(prm, noise_configs, &wx, &wy, &wz));
    }
    // transition entre valley et mountain
    else if (st2 > 0.01f && st2 < 0.99f)
    {
        noise = (st2 * computeMountainNoise(prm, noise_configs, &wx, &wy, &wz) + (1.0f - st2) * computeValleyNoise(prm, noise_configs, &wx, &wy, &wz));
        cave_noise = (st2 * computeMountainCaveNoise(prm, noise_configs, &wx, &wy, &wz) + (1.0f - st2) * computeValleyCaveNoise(prm, noise_configs, &wx, &wy, &wz));
    }
    else if (st1 > 0.99f && st2 > 0.99f)
    {
        noise = computeMountainNoise(prm, noise_configs, &wx, &wy, &wz);
        cave_noise = computeMountainCaveNoise(prm, noise_configs, &wx, &wy, &wz);
    }
    else if (st1 < 0.01f && st2 < 0.01f)
    {
        noise = computeVolcanoNoise(prm, noise_configs, &wx, &wy, &wz);
        cave_noise = computeVolcanoCaveNoise(prm, noise_configs, &wx, &wy, &wz);
    }
    else if (st1 > 0.99f && st2 < 0.01f)
    {
        noise = computeValleyNoise(prm, noise_configs, &wx, &wy, &wz);
        cave_noise = computeValleyCaveNoise(prm, noise_configs, &wx, &wy, &wz);
    }

    chunk[i] = AIR;
    if (wy > 0.0f)
    {
        cave_noise /= (wy / (/*y max*/8.0f * /*chunk size*/CHUNK_SIZE * 0.3f));
        noise /= (wy / (/*y max*/8.0f * /*chunk size*/CHUNK_SIZE * 0.3f));
        if (noise > 0.85f)
        {
            if (noise < 0.95f && cave_noise > 0.38f)
            {
                if (biome_noise > 0.28f && biome_noise < 0.66f)
                {
                    if (wy > WATER_HEIGHT)
                    {
                        float treeNoise = computeTreeNoise(prm, noise_configs, &wx, &wy, &wz);
                        if (treeNoise < 0.3f)
                            chunk[i] = TREE_SEED;
                        else if (treeNoise < 0.4f)
                            chunk[i] = WEED;
                        else
                            chunk[i] = DIRT;
                    }
                    else
                        chunk[i] = SAND;
                }
            }
            else if (noise < 0.95f)
            {
                if (wy > (WATER_HEIGHT + (BEACH_MAX_HEIGHT - 1)))
                {
                    if (biome_noise > 0.68f)
                    {
                        chunk[i] = DIRT;
                    }
                    else if (biome_noise < 0.33f)
                    {
                        chunk[i] = STONE;
                    }
                    else
                    {
                        chunk[i] = DIRT;
                    }
                }
                else
                {
                    chunk[i] = SAND;
                }
            }
            else
            {
                if (cave_noise > 0.80f)
                {
                    if (wy > 62.0f)
                        chunk[i] = COAL;
                    else
                        chunk[i] = WATER;
                }
                else if (cave_noise > 0.55f)
                {
                    chunk[i] = STONE;
                }
                else if (cave_noise > 0.35f)
                    chunk[i] = DIRT;
                else
                    chunk[i] = AIR;
            }
        }
    }
}

float
octave_noise_3d(__constant int *prm, __constant noise_config *nc,
                float const x, float const y, float const z)
{
    float				maxAmplitude =  0;
    float				total = 0;
    float				frequency_acc;
    float				amplitude_acc;
    int					i;

    frequency_acc = nc->frequency;
    amplitude_acc = nc->amplitude;
    for(i = 0; i < nc->octaves; ++i)
    {
        total += scaled_raw_noise_3d(prm, 0.0f, 1.0f, x * frequency_acc, y * frequency_acc, z * frequency_acc) * amplitude_acc;
        maxAmplitude += amplitude_acc;
        frequency_acc *= nc->lacunarity;
        amplitude_acc *= nc->persistence;
    }
    return ((total / maxAmplitude));
}

float
scaled_raw_noise_3d(__constant int *prm, float const loBound, float const hiBound, float const x, float const y, float const z)
{
    return raw_noise_3d(prm, x, y, z) * (hiBound - loBound) * 0.5f + (hiBound + loBound) * 0.5f;
}

float
raw_noise_3d(__constant int *prm, float x, float y, float z)
{
    float g[12][3] = {
        { 1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f}, { 1.0f,-1.0f, 0.0f}, {-1.0f,-1.0f, 0.0f},
        { 1.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 1.0f}, { 1.0f, 0.0f,-1.0f}, {-1.0f, 0.0f,-1.0f},
        { 0.0f, 1.0f, 1.0f}, { 0.0f,-1.0f, 1.0f}, { 0.0f, 1.0f,-1.0f}, { 0.0f,-1.0f,-1.0f}
    };

    float n0, n1, n2, n3;

    float s = (x + y + z) * 1.0f / 3.0f;
    int i = fastfloor(x + s);
    int j = fastfloor(y + s);
    int k = fastfloor(z + s);

    float G3 = 1.0f / 6.0f;
    float t = (i + j + k)*G3;
    float X0 = i - t;
    float Y0 = j - t;
    float Z0 = k - t;
    float x0 = x - X0;
    float y0 = y - Y0;
    float z0 = z - Z0;

    int i1, j1, k1;
    int i2, j2, k2;

    if (x0 >= y0)
    {
        if (y0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
        else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; }
        else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; }
    }
    else
    {
        if (y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; }
        else if (x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; }
        else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
    }

    float x1 = x0 - i1 + G3;
    float y1 = y0 - j1 + G3;
    float z1 = z0 - k1 + G3;
    float x2 = x0 - i2 + 2.0f * G3;
    float y2 = y0 - j2 + 2.0f * G3;
    float z2 = z0 - k2 + 2.0f * G3;
    float x3 = x0 - 1.0f + 3.0f * G3;
    float y3 = y0 - 1.0f + 3.0f * G3;
    float z3 = z0 - 1.0f + 3.0f * G3;

    int ii = i % 255;
    int jj = j % 255;
    int kk = k % 255;
    int gi0 = prm[ii + prm[jj + prm[kk]]] % 12;
    int gi1 = prm[ii + i1 + prm[jj + j1 + prm[kk + k1]]] % 12;
    int gi2 = prm[ii + i2 + prm[jj + j2 + prm[kk + k2]]] % 12;
    int gi3 = prm[ii + 1 + prm[jj + 1 + prm[kk + 1]]] % 12;

    float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
    if (t0 < 0.0f) n0 = 0.0f;
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * (g[gi0][0] * x0 + g[gi0][1] * y0 + g[gi0][2] * z0);
    }

    float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
    if (t1 < 0.0f) n1 = 0.0f;
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * (g[gi1][0] * x1 + g[gi1][1] * y1 + g[gi1][2] * z1);
    }

    float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
    if (t2 < 0.0f) n2 = 0.0f;
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * (g[gi2][0] * x2 + g[gi2][1] * y2 + g[gi2][2] * z2);
    }

    float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
    if (t3 < 0.0f) n3 = 0.0f;
    else
    {
        t3 *= t3;
        n3 = t3 * t3 * (g[gi3][0] * x3 + g[gi3][1] * y3 + g[gi3][2] * z3);
    }

    return (32.0f * (n0 + n1 + n2 + n3));
}

int
fastfloor(float const x)
{
    if (x > 0) return (int)x;
    else return (int)x - 1;
}
