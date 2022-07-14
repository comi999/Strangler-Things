#include "SplashScreenController.hpp"

DefineShader( Fragment_Splash )
{
	Uniform( Sampler2D, texture_diffuse );
	Uniform( float, intensity );
	Varying_In( Vector2, Texel );

	 Vector4 DiffuseColour = Rendering::Sample( texture_diffuse, Texel );
	 Rendering::FragColour = DiffuseColour * intensity;
};