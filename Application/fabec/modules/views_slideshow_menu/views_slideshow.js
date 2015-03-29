
/**
 *  @file
 *  A simple jQuery menu Div Slideshow Rotator.
 */

/**
 *  This will set our initial behavior, by starting up each individual slideshow.
 */
Drupal.behaviors.viewsSlideshowMenu = function (context) {
  $('.views_slideshow_menu_main:not(.viewsMenu-processed)', context).addClass('viewsSlideshowMenu-processed').each(function() {
    var fullIdName = $(this).attr('id');
    var fullId = '#' + fullIdName;
    var settings = Drupal.settings.viewsSlideshowMenu[fullId];
    var targetIdName = $(fullId + " :first").attr('id');
    settings.targetId = '#' + targetIdName;
    
    if ($(settings.menu_selector).length) {
      settings.opts = {
        speed:settings.speed,
        timeout:parseInt(settings.timeout),
        delay:parseInt(settings.delay),
        sync:settings.sync==1,
        pause:settings.pause==1,
        random:settings.random==1,
        pager:settings.menu_selector,
        pagerEvent: 'mouseover',
        activePagerClass: 'activeSlideMenu',
        allowPagerClickBubble: true,
        pauseOnPagerHover: true,
        pagerAnchorBuilder: function(idx, slide) {
          return settings.menu_selector + ' li.slideshow-menu-item:eq(' + idx + ')';
        },
        before:function(curr, next, opts) {
          var numMenuItems = $(settings.menu_selector + ' li.slideshow-menu-item').size();
          var currentSlide = opts.nextSlide;
          if (!$(settings.menu_selector + ' ul').hasClass('views-slideshow-menu-processed')) {
            $(settings.menu_selector + ' ul').addClass('views-slideshow-menu-processed');
            var currentSlide = 0;
          }
  
          if (currentSlide >= numMenuItems) {
            while (currentSlide >= numMenuItems) {
              currentSlide -= numMenuItems;
            }
          }
  
          $(settings.menu_selector + ' li').removeClass('activeSlideMenu');
          $(settings.menu_selector + ' li.slideshow-menu-item:eq(' + currentSlide + ')').addClass('activeSlideMenu');
        },
        cleartype:(settings.ie.cleartype),
        cleartypeNoBg:(settings.ie.cleartypenobg)
      }

      if (settings.effect == 'none') {
        settings.opts.speed = 1;
      }
      else {
        settings.opts.fx = settings.effect;
      }

      /**
       * Add additional settings.
       */
      var advanced = settings.advanced.split("\n");
      for (i=0; i<advanced.length; i++) {
        var prop = '';
        var value = '';
        var property = advanced[i].split(":");
        for (j=0; j<property.length; j++) {
          if (j == 0) {
            prop = property[j];
          }
          else if (j == 1) {
            value = property[j];
          }
          else {
            value += ":" + property[j];
          }
        }
        settings.opts[prop] = value;
      }

      $(settings.menu_selector + ' li ul li').addClass('not-slideshow-menu-item');
      $(settings.menu_selector + ' li:not(.not-slideshow-menu-item)').addClass('slideshow-menu-item');
      $(settings.menu_selector + ' li.not-slideshow-menu-item').removeClass('not-slideshow-menu-item');

      // Make the slide link to the menu location.  
      if (settings.wrap_slide_link) {
        var numSlides = $('.views_slideshow_menu_slide').size();
        var numMenuItems = $(settings.menu_selector + ' li.slideshow-menu-item').size();
    
        $(settings.menu_selector + ' li.slideshow-menu-item a').each(function(i) {
          var slideCount = i;
          while (slideCount < numSlides) {
            var link = $(this).attr('href');
            $(settings.div_prefix + settings.id + '_' + slideCount).children().wrapAll('<a href="' + link + '"></a>');
            slideCount += numMenuItems;
          }
        });
      }
      
      // Set the starting slide to the slide related to the current path.
      if (settings.start_slide_path) {
        $(settings.menu_selector + ' li.slideshow-menu-item a').each(function(slideCount) {
          if (location.pathname == $(this).attr('href')) {
            settings.opts.startingSlide = slideCount;
            return false;
          }
        });
      }
      
      $(settings.targetId).cycle(settings.opts);
    }
  });
}
