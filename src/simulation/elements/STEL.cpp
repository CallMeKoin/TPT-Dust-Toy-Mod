#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_STEL()
{
	Identifier = "DEFAULT_DT_STEL";
	Name = "STEL";
	Colour = 0x707070_rgb;
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 75;

	Weight = 100;

	HeatConduct = 251;
	Description = "Steel, doesn't rust as easily as iron.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1370.0f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].life)
		return 0;
	for (auto rx = -1; rx <= 1; rx++)
	{
		for (auto ry = -1; ry <= 1; ry++)
		{
			if (rx || ry)
			{
				auto r = pmap[y+ry][x+rx];
				switch (TYP(r))
				{
				case PT_SALT:
					if (sim->rng.chance(1, 47))
						sim->part_change_type(i,x,y,PT_BMTL);
	                    parts[i].tmp = sim->rng.between(20, 29);
					break;
				case PT_SLTW:
					if (sim->rng.chance(1, 67))
						sim->part_change_type(i,x,y,PT_BMTL);
	                    parts[i].tmp = sim->rng.between(20, 29);
					break;
                case PT_ACID:
                    if (sim->rng.chance(1,3))
						sim->part_change_type(i,x,y,PT_BMTL);
	                    parts[i].tmp = sim->rng.between(20, 29);
                    break;
				default:
					break;
				}
			}
		}
	}
	return 0;
	return 0;
}
