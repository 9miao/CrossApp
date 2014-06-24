# $URL: https://pyisapie.svn.sourceforge.net/svnroot/pyisapie/Tags/1.1.0-rc4/PyISAPIe/Python/Examples/Django/Isapi.py $
# $Rev: 162 $ $Date: 2009-05-28 13:37:33 -0700 (Thu, 28 May 2009) $
# (C)2008 Phillip Sitbon <phillip@sitbon.net>
#
"""Global ISAPI request handler.

This serves up the Django site 'mysite' via WSGI. Currently the core
handler pyisapie.py is not up to date.
"""
from django.core.handlers.wsgi import WSGIHandler as DjangoHandler
from Http.WSGI import RunWSGI
from Http import Env
import os

# Change this!
os.environ["DJANGO_SETTINGS_MODULE"] = "WWW.mysite.settings"

# This is how the WSGI module determines what part of the path
# SCRIPT_NAME should consist of. If you configure PyISAPIe as
# a wildcard map on the root of your site, you can leave this
# value as-is.
# 
Base = "/"

# This is an example of what paths might need to be handled by
# other parts of IIS that still come here first. This value's
# default of "/media" assumes that you've mapped a virtual
# directory to Django's admin media folder and so expect the
# files to be served by the static file handler.
#
Exclude = ["/media"]

# The main request handler.
# This object can be re-created for every request if desired.
#
Handler = DjangoHandler()

def Request():
  PathInfo = Env.PATH_INFO
  
  # There is no way to test if this ISAPI extension is configured
  # as a wildcard handler, so this script will fail if it is not.
  # If you'd rather have it as a script map, remove the checks below.
  #
  # You can also remove it if you set up this instance as a handler
  # for a virtual directory and know that Base will always start
  # with it. For example, if "/django_site_1" is the virtual directory
  # you're running in, and Base is set to the same value, no need
  # to ever pass control away from this handler.
  
  # Pass through to the next handler if it's not
  # part of our Django app.
  #
  if not PathInfo.startswith(Base):
    return True

  # Check for anything we know shouldn't be handled by Django and
  # pass it back to IIS, which in most cases sends it to the static
  # file handler.
  #
  for Excl in Exclude:
    if PathInfo.startswith(Excl):
      return True

  return RunWSGI(Handler, Base=Base)