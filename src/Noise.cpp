#include "../inc/Noise.hpp"
#include <math.h>
#include <random>

Noise::Noise(uint32_t const &arg_seed, uint32_t const &arg_prm_size, int const &configs_number)
	: seed(arg_seed), prm_size(arg_prm_size)
{
	uint32_t		i;
	int				swap_i;
	int				tmp;
	uint32_t const	total = prm_size * 2;

    std::mt19937 gen(arg_seed);
    std::uniform_int_distribution<> dis(0, 256);
	prm = std::unique_ptr<int[]>{ new int[total] };
	for (i = 0; i < prm_size; ++i)
		prm[i] = i;
	for (i = 0; i < prm_size; ++i)
	{
		swap_i = dis(gen) % prm_size;
		tmp = prm[swap_i];
		prm[swap_i] = prm[i];
		prm[i] = tmp;
	}
    for (i = prm_size; i < total; ++i)
        prm[i] = prm[i - prm_size];

    std::cerr << "permutation table: ";
    for (i = prm_size; i < total; ++i)
        std::cerr << prm[i] << " ";
    std::cerr << std::endl;

    this->configs_number = configs_number;
	configs = std::unique_ptr<NoiseConfig[]>{ new NoiseConfig[configs_number] };
}

Noise::~Noise() = default;

float
Noise::fade(float const &t)
{
	return (t * t * t * (t * (t * 6 - 15) + 10));
}

float
Noise::lerp(float const &t, float const &a, float const &b)
{
	return (a + t * (b - a));
}

float
Noise::grad(int const hash, float const &x, float const &y, float const &z)
{
	int							h;
	float						u;
	float						v;
	float						r;

	h = hash & 15;
	r = h == 12 || h == 14 ? x : z;
	u = h < 8 ? x : y;
	v = h < 4 ? y : r;
	if ((h & 1) == 0)
		r = u;
	else
		r = -u;
	if ((h & 2) == 0)
		r += v;
	else
		r += -v;
	return (r);
}

float
Noise::perlin(float x, float y, float z)
{
	int vx, vy, vz;
	float ux, uy, uz;
    int ax, ay, az;
    int bx, by, bz;

	vx = (int)(floorf(x)) & 255;
	vy = (int)(floorf(y)) & 255;
	vz = (int)(floorf(z)) & 255;
	x -= floorf(x);
	y -= floorf(y);
	z -= floorf(z);
	ux = fade(x);
	uy = fade(y);
	uz = fade(z);
	ax = prm[vx] + vy;
	ay = prm[ax] + vz;
	az = prm[ax + 1] + vz;
	bx = prm[vx + 1] + vy;
	by = prm[bx] + vz;
	bz = prm[bx + 1] + vz;
	return (lerp(uz, lerp(uy, lerp(ux, grad(prm[ay], x, y, z),
			grad(prm[by], x - 1, y, z)),
			lerp(ux, grad(prm[az], x, y - 1, z),
			grad(prm[bz], x - 1, y - 1, z))),
			lerp(uy, lerp(ux, grad(prm[ay + 1], x, y, z - 1),
			grad(prm[by + 1], x - 1, y, z - 1)),
			lerp(ux, grad(prm[az + 1], x, y - 1, z - 1),
			grad(prm[bz + 1], x - 1, y - 1, z - 1)))));
}

// ---------------------------------------------------------------------------------
// SIMPLEX NOISE
// ---------------------------------------------------------------------------------



inline static int
fastfloor(float const &x)
{
	return x > 0 ? (int) x : (int) x - 1;
}
/*
inline static float
dot(int const *g, float const &x, float const &y)
{
	return g[0] * x + g[1] * y;
}
*/
inline static float
dot(int const *g, float const &x, float const &y, float const &z)
{
	return g[0] * x + g[1] * y + g[2] * z;
}
/*
inline static float
dot(int const *g, float const &x, float const &y, float const &z, float const &w)
{
	return g[0] * x + g[1] * y + g[2] * z + g[3] * w;
}
*/
// 3D Scaled Multi-octave Simplex noise.
//
// Returned value will be between loBound and hiBound.
float
Noise::scaled_octave_noise_3d(int const &ci, float const &loBound, float const &hiBound, float const &x, float const &y, float const &z)
{
    return octave_noise_3d(ci, x, y, z) * (hiBound - loBound) * 0.5f + (hiBound + loBound) * 0.5f;
}

// 3D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float
Noise::scaled_raw_noise_3d(float const &loBound, float const &hiBound, float const &x, float const &y, float const &z)
{
    return raw_noise_3d(x, y, z) * (hiBound - loBound) * 0.5f + (hiBound + loBound) * 0.5f;
}

float
Noise::raw_noise_3d(float const &x, float const &y, float const &z)
{
	static const int grad3[12][3] =
	{
    	{ 1, 1, 0}, {-1, 1, 0}, { 1,-1, 0}, {-1,-1, 0},
    	{ 1, 0, 1}, {-1, 0, 1}, { 1, 0,-1}, {-1, 0,-1},
    	{ 0, 1, 1}, { 0,-1, 1}, { 0, 1,-1}, { 0,-1,-1}
	};

    float n0, n1, n2, n3; // Noise contributions from the four corners

    // Skew the input space to determine which simplex cell we're in
    float const s = (x + y + z) * 1.0f / 3.0f; // Very nice and simple skew factor for 3D
    int const i = fastfloor(x + s);
    int const j = fastfloor(y + s);
    int const k = fastfloor(z + s);

    float const G3 = 1.0f / 6.0f; // Very nice and simple unskew factor, too
    float const t = (i + j + k)*G3;
    float const X0 = i - t; // Unskew the cell origin back to (x,y,z) space
    float const Y0 = j - t;
    float const Z0 = k - t;
    float const x0 = x - X0; // The x,y,z distances from the cell origin
    float const y0 = y - Y0;
    float const z0 = z - Z0;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
    int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

    if (x0 >= y0)
    {
        if (y0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } // X Y Z order
        else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } // X Z Y order
        else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } // Z X Y order
    }
    else
    { // x0 < y0
        if (y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } // Z Y X order
        else if (x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } // Y Z X order
        else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } // Y X Z order
    }

    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.
    float const x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    float const y1 = y0 - j1 + G3;
    float const z1 = z0 - k1 + G3;
    float const x2 = x0 - i2 + 2.0f * G3; // Offsets for third corner in (x,y,z) coords
    float const y2 = y0 - j2 + 2.0f * G3;
    float const z2 = z0 - k2 + 2.0f * G3;
    float const x3 = x0 - 1.0f + 3.0f * G3; // Offsets for last corner in (x,y,z) coords
    float const y3 = y0 - 1.0f + 3.0f * G3;
    float const z3 = z0 - 1.0f + 3.0f * G3;

    // Work out the hashed gradient indices of the four simplex corners
    int const ii = i & 255;
    int const jj = j & 255;
    int const kk = k & 255;
    int const gi0 = prm[ii + prm[jj + prm[kk]]] % 12;
    int const gi1 = prm[ii + i1 + prm[jj + j1 + prm[kk + k1]]] % 12;
    int const gi2 = prm[ii + i2 + prm[jj + j2 + prm[kk + k2]]] % 12;
    int const gi3 = prm[ii + 1 + prm[jj + 1 + prm[kk + 1]]] % 12;

    // Calculate the contribution from the four corners
    float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
    if (t0 < 0.0f) n0 = 0.0f;
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
    }

    float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
    if (t1 < 0.0f) n1 = 0.0f;
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
    }

    float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
    if (t2 < 0.0f) n2 = 0.0f;
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
    }

    float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
    if (t3 < 0.0f) n3 = 0.0f;
    else
    {
        t3 *= t3;
        n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return (32.0f * (n0 + n1 + n2 + n3));
}

// 3D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float
Noise::octave_noise_3d(int const &ci, float const &x, float const &y, float const &z)
{
    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    float				maxAmplitude =  0;
    float				total = 0;
    float				frequency;
    float				amplitude;
    int					i;
	NoiseConfig			*n = &configs[ci];

    frequency = n->frequency;
    amplitude = n->amplitude;
    for(i = 0; i < n->octaves; ++i)
    {
        total += this->scaled_raw_noise_3d(0, 1, x * frequency, y * frequency, z * frequency) * amplitude;
        maxAmplitude += amplitude;
        frequency *= n->lacunarity;
        amplitude *= n->persistence;
    }
	return ((total / maxAmplitude));
} 

float
Noise::fractal(int const &ci, float const &x, float const &y, float const &z)
{
	float				maxAmplitude = 0;
	float				total = 0.0f;
	float				frequency;
	float				amplitude;
	int					i;
	NoiseConfig			*n = &configs[ci];

	frequency = n->frequency;
	amplitude = n->amplitude;
	for (i = 0; i < n->octaves; ++i)
	{
		total += this->perlin(x * frequency, y * frequency, z * frequency) * amplitude;
		maxAmplitude += amplitude;
		frequency *= n->lacunarity;
		amplitude *= n->persistence;
	}
	return (total / maxAmplitude);
}

Noise
&Noise::operator=(Noise const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Noise const &i)
{
	o	<< "Noise: " << &i;
	return (o);
}
