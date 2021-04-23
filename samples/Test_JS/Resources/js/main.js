require('js/rootwindow.js');
require('js/require.js');

var window = new RootWindow();
window.init();
ca.application.runWindow(window);
window.release();
ca.rootWindow = window;
