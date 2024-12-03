uniform sampler2D texture;
uniform int threshhold;
uniform int iterations;
uniform float zoom;
uniform float x_coord;
uniform float y_coord;

//x is real component, y is imaginary component
vec2 squareComplex(vec2 num) {
	vec2 result;
	result.x = pow(num.x, 2.0) - pow(num.y, 2.0);
	result.y = 2*num.x*num.y;
	return result;
}

vec2 addComplex(vec2 num1, vec2 num2) {
	vec2 result;
	result.x = num1.x + num2.x;
	result.y = num1.y + num2.y;
	return result;
}

float magComplex(vec2 num) {
	return sqrt(pow(num.x, 2.0) + pow(num.y, 2.0));
}

vec4 mandlebrot(float x, float y) {

	vec4 color;
	color.rgba = vec4(0.0, 0.0, 0.0, 1.0);

	vec2 z;
	z.xy = vec2(0.0);
	vec2 c;
	c.xy = vec2((float(x) - 0.5) * 4.0 / zoom + x_coord, (float(y) - 0.5) * 4.0 / zoom  + y_coord);
	for (int i = 0; i < iterations; i++) {
		z = addComplex(squareComplex(z), c);
		if (magComplex(z) > threshhold) {
			color.rgba = vec4(float(i) / float(iterations), 0.1, 0.2, 1.0);
			return color;
		}
	}
	return color;
}

void main()
{
    gl_FragColor.rgba = mandlebrot(gl_TexCoord[0].x, gl_TexCoord[0].y).rgba;
}

