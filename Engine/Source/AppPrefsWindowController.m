//
//  AppPrefsWindowController.m
//


#import "AppPrefsWindowController.h"
#include <SDL/SDL.h>

@implementation AppPrefsWindowController




- (void)setupToolbar
{
	
	[self addView:generalPreferenceView label:@"General" image:[NSImage imageNamed:@"NSPreferencesGeneral"]];
	[self addView:displayPreferenceView label:@"Display" image:[NSImage imageNamed:@"Displays"]];
	[self addView:graphicsPreferenceView label:@"Graphics" image:[NSImage imageNamed:@"Graphics"]];
	
	[self setCrossFade:YES];
	[self setShiftSlowsAnimation:YES];
		
	SDL_Rect ** modes;

	modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);
	
	if (modes == (SDL_Rect **)0) {
		NSLog(@"No modes available!\n");
	}
	else if(modes == (SDL_Rect **)-1) {
		NSLog(@"All resolutions available.\n");
	}
	else {
		int i;
		
		for (i = 0; modes[i]; ++i) {
			NSString * formatString = [NSString stringWithFormat:@"%ix%i", modes[i]->w, modes[i]->h];
			[resolutionList addItemWithTitle:formatString];
		}
	}
}




@end
