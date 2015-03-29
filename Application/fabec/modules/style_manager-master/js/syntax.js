(function ($) {
  $.fn.extend({
    syntaxCSS: function (d) {
      return extend_run(this, $.syntax, 'CSS', d);
    }
  });
  $.syntax = {
    CSS: function (css, decorator) {
      decorator = $.extend({}, decorators.CSS, decorator);
      var comments = [];
      return decorator.block(
        decorator.lines(
          css.replace(/\/\*([\s\S]*?)\*\//g, function (m, t) {
            return '\0C' + push(comments, multiline_comments(m, decorator.rem)) + '\0';
          })
            .replace(/([\.#:]{0,1}[a-z0-9_]+[^\{~\0]*)(\{)([^}]*)(\})/gi, function (m, sel, o, p, c) {
              return  multiline_comments(sel, decorator.sel) + o + SyntaxCSS_param(p, decorator) + c;
            })
            .replace(/\0C(\d+)\0/g, function (m, i) {
              return comments[i];
            })
            .split('\n')
        )
      );
    }
  };
  var decorators = {
    CSS: {
      rem: function (txt) {
        return '<span class="rem">'.concat(txt, '</span>');
      },
      sel: function (txt) {
        return '<span class="sel">'.concat(txt, '</span>');
      },
      param: function (n, v) {
        return '<span class="name">'.concat(n, '</span>:<span class="value">', v, '</span>');
      },
      lines: function (lines) {
        return lines.join('<br/>');
      },
      block: function (txt) {
        return '<div class="css">'.concat(txt.replace(/  /g, '&nbsp;&nbsp;'), '</div>');
      }
    }
  };
  var cssre = /\bJS|CSS|XML|HTML|CS\b/i;
  var safe = { '<': '&lt;', '>': '&gt;', '&': '&amp;' };
  var htmlen = { '&lt;': '<', '&gt;': '>', '&amp;': '&', '&quot;': '"' };
  function getSafe(c) {
    return safe[c];
  }
  function multiline_comments(txt, decorator) {
    txt = txt.split('\n');
    for (var i = 0; i < txt.length; i++) {
      txt[i] = decorator(txt[i]);
    }
    return txt.join('\n');
  }
  function push(arr, e) {
    arr.push(e);
    return arr.length - 1;
  }
  function SyntaxCSS_param(txt, decorator) {
    return txt.replace(/([^:\n]+):([^;]*)(;)?/g, function (m, n, v, e) {
      return decorator.param(n, v).concat(e == null ? '' : e);
    });
  }
  function extend_run(arr, obj, syntax, decorator) {
    var i;
    syntax = obj[syntax];
    return arr.each(function () {
      i = $(this);
      i.html(syntax.call(obj, i.text(), decorator));
    });
  }
})(jQuery);
