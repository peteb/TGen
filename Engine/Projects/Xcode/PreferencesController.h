#import <Cocoa/Cocoa.h>

@interface PreferencesController : NSWindowController {

	IBOutlet NSWindow * myWindow;
	IBOutlet NSView * generalPane;
	IBOutlet NSView * freshPane;

	IBOutlet NSToolbar * myToolbar;
	IBOutlet NSToolbarItem * generalItem, * freshItem;
	
@private
	NSString * name;

}

- (void) awakeFromNib;
- (void) runWindow;
- (void) setName:(NSString *)newName;
- (IBAction)printStuff:(id)sender;
- (IBAction)selectGeneralPane:(id)sender;
- (IBAction)selectFreshPane:(id)sender;
- (NSArray *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar;

@end
