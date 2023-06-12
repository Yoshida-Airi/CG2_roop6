struct PixelShaderOutput
{
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main()
{
	PixelShaderOutput output;
	output.color = float32_t4(1.0, 1.0, 1.0, 1.0);	//白色
	return output;
}