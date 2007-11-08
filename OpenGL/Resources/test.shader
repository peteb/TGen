textures/base_wall/metalfloor_wall_15ow
{
       {
			clampmap pent_glow.png
            blendFunc GL_ONE GL_ZERO
           // rgbGen wave sin 0.5 0.5 0 1
			tcmod rotate 180
        }
	
        /*{
			map hologirl.tga
            tcmod scale 2 .4
            tcmod scroll 6 .6
            //tcMod turb 0 .1 0 .01
            blendFunc GL_ONE GL_ONE
            rgbGen identity
                
		}
		{
			map metalfloor_wall_15ow.tga
            blendfunc blend
			rgbGen identity
		}
        {
			map $lightmap
			blendFunc filter
            //blendfunc dst_color gl_zero                                             
		}*/
}

testfont
{
	{
		rgbgen vertex
		map smallfont.tga
	}
}
