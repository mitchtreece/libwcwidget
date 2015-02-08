// WCWidget.h
//
// Copyright (c) 2015 Mitch Treece (http://mitchtreece.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
Constants indicating where on the screen WidgetCenter is being presented.
*/
typedef NS_ENUM(NSInteger, WCPosition) {
    /**
    Indicates the top of the screen.
    */
    WCPositionTop,
    /**
    Indicates the bottom of the screen.
    */
    WCPositionBottom
};

/**
Constants indicating the effect to use on the hosting app's window.
*/
typedef NS_ENUM(NSInteger, WCEffect) {
    /**
    Indicates no effect.
    */
    WCEffectNone,
    /**
    Indicates a darkening effect.
    */
    WCEffectDarken,
    /**
    Indicates a blur effect.
    */
    WCEffectBlur
};

/**
Constants indicating the widget's dismissal context.
*/
typedef NS_ENUM(NSInteger, WCWidgetDismissContext) {
    /**
    Indicates the default context (no animation).
    */
    WCWidgetDismissContextNone,
    /**
    Indicates a tapped context (zoom animation).
    */
    WCWidgetDismissContextTapped
};

/**
Constant indicating a void animation block.
*/
typedef void (^WCWidgetAnimations)(void);

/**
Constant indicating a void completion block.
*/
typedef void (^WCWidgetCompletion)(void);

/**
The WCWidget class is the base class of every WidgetCenter widget.
*/
@interface WCWidget : UIView

/**---------------------------------------------------------------------------------------
* @name Properties
*  ---------------------------------------------------------------------------------------
*/

/**
The widget's height. This should be explicitly set during widget instantiation.
@warning Keep in mind, the user navigates WidgetCenter via gestures on the hosting app's window. If the widget's height is >= the screen height, the hosting app window will not be seen; and the user will be unable to interact with WidgetCenter. THIS IS BAD.
*/
@property (nonatomic) CGFloat height;

/**
The widget's hostView.
@warning The hostView should not be modified in ANY way. It provides widget size information (i.e. hostView.frame.size.width).
*/
@property (nonatomic, weak, readonly) UIView *hostView;

/**
WidgetCenter's display (screen) position.
@warning The position property should not be modified in ANY way. It provides the widget information regarding where on the screen it will be presented.
*/
@property (nonatomic, readonly) WCPosition position;

/**
The current application's bundleIdentifier.
@warning The hostingAppIdentifier property should not be modified in ANY way. It provides the widget information about the current running app.
*/
@property (nonatomic, readonly) NSString *hostingAppIdentifier;

/**
The effect to use on the hosting app's window. Defaults to WCEffectDarken.
*/
@property (nonatomic, assign) WCEffect hostingAppEffect;

/**
A BOOL representing whether the widget's alpha should be modified during animations. Defaults to YES.
@warning This should usually be left at it's default value. However, it's useful to override if the widget uses special (blurry) views.
*/
@property (nonatomic, assign) BOOL shouldAnimateAlpha;

/**---------------------------------------------------------------------------------------
* @name Widget Initalization
*  ---------------------------------------------------------------------------------------
*/

/** Initializes and returns a newly allocated widget object for the calling hostView.

The entry point for every WCWidget.

@warning initWithHostView: should NEVER be directly called. Instead, it should be overriden in a custom WCWidget subclass. Construct your widget and return it here.
@param hostView The widget's hosting view.
@return An initialized WCWidget object, or nil if the object couldn't be created.
*/
- (WCWidget *)initWithHostView:(UIView *)hostView;

/**---------------------------------------------------------------------------------------
* @name Widget Appearance/Disappearance
*  ---------------------------------------------------------------------------------------
*/

/** Returns whether the widget should appear.

Defaults to YES. Override this method to present a widget only in certain situations.
(i.e. Only show widget if certain app is open).

@return A BOOL indicating whether the widget should appear.
*/
- (BOOL)widgetShouldAppear;

/** Called whenever the widget is about to be displayed.

Override this method to do additional tasks before the widget appears.

@param animated A BOOL indicating whether the widget will animate it's appearance.
*/
- (void)widgetWillAppear:(BOOL)animated;

/** Called whenever the widget is about to disappear.

Override this method to do additional tasks before the widget disappears.

@param animated A BOOL indicating whether the widget will animate it's disappearance.
*/
- (void)widgetWillDisappear:(BOOL)animated;

/**---------------------------------------------------------------------------------------
* @name Widget Height
*  ---------------------------------------------------------------------------------------
*/

/** Updates the widget's height and notifies the hostView.

@param height A CGFloat representing the new widget height.
@param animated A BOOL indicating whether the widget should animate the height change.
@param animations An animation block that returns void. Additional animations can be done here alongside the height change.
@param completion A completion block that returns void. Called as soon as the widget finshes updating it's height.
*/
- (void)updateWidgetHeight:(CGFloat)height animated:(BOOL)animated withAnimations:(WCWidgetAnimations)animations completion:(WCWidgetCompletion)completion;

/**---------------------------------------------------------------------------------------
* @name WidgetCenter Dismissal
*  ---------------------------------------------------------------------------------------
*/

/** Dismisses WidgetCenter with default context.

Dismisses WidgetCenter immediately.

@see dismissWidgetCenterWithContext:completion:
*/
- (void)dismissWidgetCenter;

/** Dismisses WidgetCenter with specified context.

Dismisses WidgetCenter with a given WCWidgetDismissContext.

@param context The WCWidgetDismissContext for dismissal.
@param completion A completion block that return void. Called as soon as WidgetCenter finishes dismissing.
@see dismissWidgetCenter
*/
- (void)dismissWidgetCenterWithContext:(WCWidgetDismissContext)context completion:(WCWidgetCompletion)completion;

/**---------------------------------------------------------------------------------------
* @name Widget Gestures
*  ---------------------------------------------------------------------------------------
*/

/** Called whenever a long-press is recognized on the hosting app's window.

Override this method to do additional tasks whenever a tap-and-hold is recognized on the hosting app's window.
*/
- (void)widgetHoldAction;

/**---------------------------------------------------------------------------------------
* @name Widget Expanding/Shrinking
*  ---------------------------------------------------------------------------------------
*/

/** Returns whether the widget can expand and shrink.

Defaults to NO. Override this method to allow widget expansion/shrinking.

@return A BOOL indicating whether the widget can expand/shrink.
*/
- (BOOL)widgetCanExpandAndShrink;

/** Returns whether the widget is currently expanded.

Override this method and return the widget's expanded state.

@return A BOOL indicating whether the widget is expanded.
*/
- (BOOL)widgetIsExpanded;

/** Called whenever an expand gesture is recognized on the hosting app's window.

Override this method with logic that resizes and/or re-layouts the widget for an expanded state.
*/
- (void)widgetExpandAction;

/** Called whenever a shrink gesture is recognized on the hosting app's window.

Override this method with logic that resizes and/or re-layouts the widget for it's default (shrunk) state.
*/
- (void)widgetShrinkAction;

/**---------------------------------------------------------------------------------------
* @name Widget Helpers
*  ---------------------------------------------------------------------------------------
*/

/** Activates an application based on it's bundleID.

@param bundleID The bundleID of the application.
*/
- (void)activateApplicationWithBundleID:(NSString *)bundleID;

/** Returns a default light widget color.

@return A light widget UIColor.
*/
- (UIColor *)lightWidgetColor;

/** Returns a default dark widget color.

@return A dark widget UIColor.
*/
- (UIColor *)darkWidgetColor;

@end
