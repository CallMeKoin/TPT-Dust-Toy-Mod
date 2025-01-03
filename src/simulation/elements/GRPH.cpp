#include "simulation/ElementCommon.h"
#include "simulation/ElementDefs.h"

int Element_GRPH_update(UPDATE_FUNC_ARGS);
int Element_GRPH_graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_GRPH()
{
	Identifier = "DEFAULT_DT_GRPH";
	Name = "GRPH";
	Colour = 0x212121_rgb;
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.0f;
	HotAir = 0.0f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;
	PhotonReflectWavelengths = 0x00000000;

	Weight = 100;

	HeatConduct = 200;
	Description = "Graphite, burns more agressively than coal. Gets red when hot.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.life = 1010;
	DefaultProperties.tmp = 50;

	Update = &Element_GRPH_update;
	Graphics = &Element_GRPH_graphics;
}

int Element_GRPH_update(UPDATE_FUNC_ARGS)
{
    for(int ry = -1; ry <= 1; ry++)
    {
        for(int rx = -1; rx <= 1; rx++)
        {
           if (rx || ry)
			{
				auto r = pmap[y+ry][x+rx];
				switch (TYP(r))
				{
				case PT_NEUT:
					//sim->part_change_type(ID(pmap[y+ry][x+rx]),x+rx,y+ry);
                    sim->kill_part(ID(pmap[y+ry][x+rx]));
					break;
                case PT_IRON:
                {
                    if(parts[i].temp >= 1600 && parts[ID(pmap[y+ry][x+rx])].temp >= 1500)
                    {
                        sim->kill_part(ID(pmap[y+ry][x+rx]));
                        sim->create_part(i, x, y, PT_STEL);
		                return 1;
                    }
                }
				default:
					break;
				}
			}
        }
    }
	if (parts[i].life<=0) {
		sim->create_part(i, x, y, PT_FIRE);
		return 1;
	} else if (parts[i].life < 1000) {
		parts[i].life--;
		sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
        sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
        sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
        if(sim->rng.chance(1, 3))
        {
            sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
            sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
            sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
            sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
            sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
            sim->create_part(-1, x + sim->rng.between(-3, 3), y + sim->rng.between(-3, 3), PT_FIRE);
        }
        
	}
	if (parts[i].type == PT_GRPH)
	{
		if ((sim->pv[y/CELL][x/CELL] > 4.3f)&&parts[i].tmp>40)
			parts[i].tmp=39;
		else if (parts[i].tmp<40&&parts[i].tmp>0)
			parts[i].tmp--;
		else if (parts[i].tmp<=0) {
			sim->part_change_type(i, x, y, PT_BCOL);
			return 1;
		}
	}
	if(parts[i].temp > parts[i].tmp2)
		parts[i].tmp2 = int(parts[i].temp);
	return 0;
}

constexpr float FREQUENCY = 3.1415f/(2*300.0f-(300.0f-200.0f));

int Element_GRPH_graphics(GRAPHICS_FUNC_ARGS)
{
	*colr += int((cpart->tmp2-295.15f)/3);

	if (*colr > 170)
		*colr = 170;
	if (*colr < *colg)
		*colr = *colg;

	*colg = *colb = *colr;

	// ((cpart->temp-295.15f) > 300.0f-200.0f)
	if (cpart->temp > 395.15f)
	{
		//  q = ((cpart->temp-295.15f)>300.0f)?300.0f-(300.0f-200.0f):(cpart->temp-295.15f)-(300.0f-200.0f);
		auto q = int((cpart->temp > 595.15f) ? 200.0f : cpart->temp - 395.15f);

		*colr += int(sin(FREQUENCY*q) * 226);
		*colg += int(sin(FREQUENCY*q*4.55 + TPT_PI_DBL) * 34);
		*colb += int(sin(FREQUENCY*q*2.22 + TPT_PI_DBL) * 64);
	}
	return 0;
}