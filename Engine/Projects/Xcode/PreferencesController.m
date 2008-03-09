#import "PreferencesController.h"

@implementation PreferencesController
- (IBAction)printStuff:(id)sender {
	NSAlert * alert = [[NSAlert alloc] init];
	
	
	NSString * message = [[NSString alloc] initWithFormat:@"Hejsan, %@!", name];
	
	[alert setMessageText:message];
	[alert runModal];
}

- (IBAction)selectGeneralPane:(id)sender {
	int diffY = [generalPane frame].size.height - [[myWindow contentView] frame].size.height;
	
	NSRect newWindowFrame = [myWindow frame];
	newWindowFrame.size.height += diffY;
	newWindowFrame.origin.y -= diffY;
	
	
	//[myWindow setContentView:nil];
	[myWindow setFrame: newWindowFrame display:YES animate:YES];
	[myWindow setContentView: generalPane];
	[myWindow setTitle:@"General"];
	//[myToolbar setSelectedItemIdentifier:[generalItem itemIdentifier]];
	
}

- (IBAction)selectFreshPane:(id)sender {
	int diffY = [freshPane frame].size.height - [[myWindow contentView] frame].size.height;
	
	NSRect newWindowFrame = [myWindow frame];
	newWindowFrame.size.height += diffY;
	newWindowFrame.origin.y -= diffY;
	
	
	//[myWindow setContentView:nil];

	[myWindow setFrame: newWindowFrame display:YES animate:YES];
	
	[myWindow setContentView: freshPane];
	[myWindow setTitle:@"Fresh"];
	//[myToolbar setSelectedItemIdentifier:[freshItem itemIdentifier]];
	
}

- (void)setName:(NSString *)newName {
	name = newName;
}

- (void)runWindow {
	[myWindow makeKeyAndOrderFront:nil];
}

- (void) awakeFromNib {
	[self runWindow];
	[myToolbar setSelectedItemIdentifier:@"General"];
}

- (NSArray *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar {
	return [NSArray arrayWithObjects:
			  [generalItem itemIdentifier],
			  [freshItem itemIdentifier],
			  nil];
}


@end

