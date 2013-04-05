//--------------------------------------------
uniform vec3 iResolution;    // viewport resolution (in pixels)
uniform float iGlobalTime;   // shader playback time (in seconds)
uniform vec4 iMouse;         // mouse pixel coords xy: current (if MLB down), zw:click
uniform sampler2D iChannel0; // input channel.
//--------------------------------------------

#define SECTOUR 3.
#define RBOULE 0.1
#define NBBOULE 6.
#define PI 3.14159265359

void boule(vec2 coord_boule, vec2 uv, int num);

void main(void)
{
        vec2 uv = gl_FragCoord.xy / iResolution.xy;

        gl_FragColor = vec4(0,0,0,0);

        float time = iGlobalTime;
        float mod_time;
        vec2 coord_boule;

        for(int i=0; i<int(NBBOULE); i++)
        {
                mod_time = mod(time+float(i)*(SECTOUR/NBBOULE), SECTOUR);
                coord_boule = vec2(cos((mod_time/SECTOUR)*2.*PI),sin((mod_time/SECTOUR)*2.*PI));
                coord_boule.x = coord_boule.x / 4. + 0.5;
                coord_boule.y = coord_boule.y / 4. + 0.5;
                boule(coord_boule, uv, int(mod(float(i), 3.)+1.));
        }

}

void boule(vec2 coord_boule, vec2 uv, int num)
{
        vec4 limiteBoule;
        limiteBoule[0] = (coord_boule.x + RBOULE);
        limiteBoule[1] = (coord_boule.x - RBOULE);
        limiteBoule[2] = (coord_boule.y + RBOULE);
        limiteBoule[3] = (coord_boule.y - RBOULE);

        if(sqrt(pow(coord_boule.x-uv.x, 2.) + pow(coord_boule.y-uv.y,2.)) <= RBOULE)
        {
                vec2 coord_rec = vec2((uv.x-limiteBoule[1])/(2.*RBOULE),(uv.y-limiteBoule[3])/(2.*RBOULE));
                float tmp = sqrt(pow(coord_rec.x-0.5, 2.) + pow(coord_rec.y-0.5,2.)) * 2.;

                float dist_centre = 1. - tmp;

                if(num==1)
                        gl_FragColor = vec4(dist_centre-tmp,dist_centre,dist_centre-tmp,1.0);
                if(num==2)
                        gl_FragColor = vec4(dist_centre-tmp,dist_centre-tmp,dist_centre,1.0);
                if(num==3)
                        gl_FragColor = vec4(dist_centre,dist_centre-tmp,dist_centre-tmp,1.0);
        }
}
