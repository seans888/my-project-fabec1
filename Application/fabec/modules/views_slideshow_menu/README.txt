-- SUMMARY --

This module associates a slideshow slide with a menu items in a designated 
menu. As the display of each new slide, the class "activeSlideMenu" is 
removed from the last menu item and applied to the next.

This is an add-on to the Views and Views Slideshow module. Configuration 
begins with creating a View with style set to "Slideshow" and filtering 
and displaying a series of slides to be associated with a particular menu.

The project page for the module is at:
  http://drupal.org/project/views_slideshow_menu

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/views_slideshow_menu

-- REQUIREMENTS --

* Views 2 (http://drupal.org/project/views) 
* Views Slideshow 2 (http://drupal.org/views_slideshow)


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Permissions. Anyone with "administer views" permissions will be able to 
  create the views slideshow menu.

* How to Use

  1) Create a slideshow using the Views Slideshow module (version 2) 
     http://drupal.org/project/views_slideshow

  2) Make sure that you have the same number of slides in the show as you 
     have menu items in the menu for the Views Slideshow Menu.

  3) Make sure the slides are displayed in the same order as the order of 
     the menu items.

  4) Click on the gear icon next to "style: Slideshow" on your view edit 
     screen.

  5) Set "Slideshow mode" to "menu".

  6) Determine whether the menu you want to use is appearing on the page via 
     a menu block or some other way.

     a) If it is via a block, then you simply choose which block menu from 
        the drop-down, "Menu block." 

     b) But it's also possible that your desired menu got on the page by being 
        put there directly in a template file. This is often the case with 
        Primary links. In that case you need to determine the class or id used 
        for that menu. Under "Menu block" you now choose, "Use selector" and 
        under "Menu selector" you put the id or class of the menu (including the 
        "#" or "."). For example it might be, ".primary-links." The "Menu block" 
        setting must be set to "Use selector" for this setting to take effect.

  7) Style the menu item associated with the active slide by applying styling to:

     .activeSlideMenu a

     For example, the snippet here would make the active menu item have 
     a yellow background:

      .activeSlideMenu a {
        background-color: yellow;
      }

  8) clear cache

-- CONTACT --

Maintainers:
* Adam Moore (sredndahead) - http://drupal.org/user/160320
* Shai Gluskin (Shai) - http://drupal.org/user/50259

This project has been sponsored by:
* Content2zero Web Development. 
  Specialized in working to provide affordable and powerful web sites and 
  training to small businesses and non-profits. Visit http://content2zero.com 
  for more information.

