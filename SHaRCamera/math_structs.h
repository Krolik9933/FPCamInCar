struct vec3f
{
	float x, y, z;
	vec3f operator+(const vec3f &other) const
	{
		vec3f val = { x + other.x, y + other.y, z + other.z };
		return val;
	}
	void operator+=(const vec3f &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
};