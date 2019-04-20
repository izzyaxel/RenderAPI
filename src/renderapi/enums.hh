#pragma once

enum struct ColorFormat
{
	RGB, RGBA32
};

enum struct InterpolationMode
{
	Nearest, Linear
};

enum struct AttachmentType
{
	Color, Alpha, Depth, Stencil,
};
