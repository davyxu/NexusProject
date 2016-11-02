#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{	
	const Color Color::cBlack  = Color(1.0f,0.0f,0.0f,0.0f);
	const Color Color::cRed    = Color(1.0f,1.0f,0.0f,0.0f);
	const Color Color::cGreen  = Color(1.0f,0.0f,1.0f,0.0f);
	const Color Color::cBlue   = Color(1.0f,0.0f,0.0f,1.0f);
	const Color Color::cYellow = Color(1.0f,1.0f,1.0f,0.0f);
	const Color Color::cWhite  = Color(1.0f,1.0f,1.0f,1.0f);

	Color::Color( ) :
		a(1.0f),r(1.0f),g(1.0f),b(1.0f)
	{
	}

	Color::Color(dword color)
	{
		SetColor( color );
	}
	
	Color::Color(float alpha, float red, float green, float blue) :
		a(alpha),r(red),g(green),b(blue)
	{
	}

	dword Color::operator = (const dword color)
	{
		SetColor(color);
		return color;
	}

	Color& Color::operator = (const Color& color)
	{
		a = color.a;
		r = color.r;
		g = color.g;
		b = color.b;

		return *this;
	}

	bool Color::operator == (const Color& color)
	{
		dword thisValue  = ToDWORD(a,r,g,b);
		dword thatValue  = ToDWORD(color.a,color.r,color.g,color.b);

		return thisValue == thatValue;
	}

	bool Color::operator != (const Color& color)
	{
		dword thisValue  = ToDWORD(a,r,g,b);
		dword thatValue  = ToDWORD(color.a,color.r,color.g,color.b);

		return thisValue != thatValue;
	}

	Color Color::Inverse(void)
	{
		Color ret;
		ret.r = 1 - r;
		ret.g = 1 - g;
		ret.b = 1 - b;
		ret.a = a;
		return ret;
	}



	Color::operator dword(void) const
	{
		return ToDWORD(a,r,g,b);
	}

	dword Color::ToDWORD( ) const
	{
		return ToDWORD(a,r,g,b);
	}

	Color& Color::SetColor(dword color)
	{
		float div_255 = 1 / 255.0f;
		a = (color >> 24) * div_255;
		r = ((color & 0x00ff0000) >>16) * div_255;
		g = ((color & 0x0000ff00) >>8) * div_255;
		b = (color & 0x000000ff) * div_255;

		return *this;
	}

	Color& Color::SetColor255(_byte alpha,_byte red,_byte green,_byte blue)
	{
		a = alpha/255.0f;
		r = red/255.0f;
		g = green/255.0f;
		b = blue/255.0f;
		return *this;
	}

	Color& Color::SetColorABGR(dword color)
	{
		float div_255 = 1 / 255.0f;
		a = (color >> 24) * div_255;
		b = ((color & 0x00ff0000) >>16) * div_255;
		g = ((color & 0x0000ff00) >>8) * div_255;
		r = (color & 0x000000ff) * div_255;
		return *this;
	}

	Color& Color::SetColor(float alpha, float red, float green, float blue)
	{
		a = alpha;
		r = red;
		g = green;
		b = blue;
		return *this;
	}


	dword Color::ToDWORD(float alpha, float red, float green, float blue)
	{
		return   ((int(alpha * 255.0f) & 0xff) << 24) | 
					((int(red * 255.0f) & 0xff) << 16)	 | 
					((int(green * 255.0f) & 0xff) << 8)  | 
					((int(blue * 255.0f) & 0xff) );
	}

	Color Color::Lerp(const Color& color1,const Color& color2,float s)
	{
		Color out;
		out.a = (color2.a - color1.a) * s + color1.a;
		out.r = (color2.r - color1.r) * s + color1.r;
		out.g = (color2.g - color1.g) * s + color1.g;
		out.b = (color2.b - color1.b) * s + color1.b;
		return out;
	}


	Color& Color::SetHSB(float Hue, float Saturation, float Brightness)
	{
		// wrap hue
		if (Hue > 1.0f)
		{
			Hue -= (int)Hue;
		}
		else if (Hue < 0.0f)
		{
			Hue += (int)Hue + 1;
		}
		// clamp saturation / brightness
		Saturation = Math::Min(Saturation, (float)1.0);
		Saturation = Math::Max(Saturation, (float)0.0);
		Brightness = Math::Min(Brightness, (float)1.0);
		Brightness = Math::Max(Brightness, (float)0.0);

		if (Brightness == 0.0f)
		{   
			// early exit, this has to be black
			r = g = b = 0.0f;
			return *this;
		}

		if (Saturation == 0.0f)
		{   
			// early exit, this has to be grey

			r = g = b = Brightness;
			return *this;
		}


		float hueDomain  = Hue * 6.0f;
		if (hueDomain >= 6.0f)
		{
			// wrap around, and allow mathematical errors
			hueDomain = 0.0f;
		}
		unsigned short domain = (unsigned short)hueDomain;
		float f1 = Brightness * (1 - Saturation);
		float f2 = Brightness * (1 - Saturation * (hueDomain - domain));
		float f3 = Brightness * (1 - Saturation * (1 - (hueDomain - domain)));

		switch (domain)
		{
		case 0:
			// red domain; green ascends
			r = Brightness;
			g = f3;
			b = f1;
			break;
		case 1:
			// yellow domain; red descends
			r = f2;
			g = Brightness;
			b = f1;
			break;
		case 2:
			// green domain; blue ascends
			r = f1;
			g = Brightness;
			b = f3;
			break;
		case 3:
			// cyan domain; green descends
			r = f1;
			g = f2;
			b = Brightness;
			break;
		case 4:
			// blue domain; red ascends
			r = f3;
			g = f1;
			b = Brightness;
			break;
		case 5:
			// magenta domain; blue descends
			r = Brightness;
			g = f1;
			b = f2;
			break;
		}

		return *this;
	}

	_byte Color::RGB32ToGray( _byte R, _byte G, _byte B )
	{
		return ( R * 3 + G * 6 + B ) / 10;
	}

	//---------------------------------------------------------------------
	void Color::GetHSB(float* Hue, float* Saturation, float* Brightness) const
	{

		float vMin = Math::Min(r, Math::Min(g, b));
		float vMax = Math::Max(r, Math::Max(g, b));
		float Delta = vMax - vMin;

		*Brightness = vMax;

		if (Math::Abs( Delta ) <= 1e-6 )
		{
			// grey
			*Hue = 0;
			*Saturation = 0;
		}
		else                                    
		{
			// a colour
			*Saturation = Delta / vMax;

			float deltaR = (((vMax - r) / 6.0f) + (Delta / 2.0f)) / Delta;
			float deltaG = (((vMax - g) / 6.0f) + (Delta / 2.0f)) / Delta;
			float deltaB = (((vMax - b) / 6.0f) + (Delta / 2.0f)) / Delta;

			if (Math::Abs( r - vMax) <= 1e-6 )
				*Hue = deltaB - deltaG;
			else if (Math::Abs( g - vMax) <= 1e-6 )
				*Hue = 0.3333333f + deltaR - deltaB;
			else if (Math::Abs( b - vMax) <= 1e-6 ) 
				*Hue = 0.6666667f + deltaG - deltaR;

			if (*Hue < 0.0f) 
				*Hue += 1.0f;
			if (*Hue > 1.0f)
				*Hue -= 1.0f;
		}


	}

}