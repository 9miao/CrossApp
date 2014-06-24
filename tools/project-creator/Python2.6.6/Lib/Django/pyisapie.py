# $URL: https://pyisapie.svn.sourceforge.net/svnroot/pyisapie/Tags/1.1.0-rc4/PyISAPIe/Python/Examples/Django/pyisapie.py $
# $Rev: 91 $ $Date: 2008-01-10 23:53:59 -0800 (Thu, 10 Jan 2008) $
# (C)2008 Phillip Sitbon <phillip@sitbon.net>
#
"""Custom request handler for PyISAPIe.
"""

from django.core.handlers.base import BaseHandler
from django.core import signals
from django.dispatch import dispatcher
from django.utils import datastructures
from django import http
import os

from Http import *

class PyISAPIeRequest(http.HttpRequest):
    def __init__(This):
        This.method = Env.REQUEST_METHOD.upper()
        This.path = Env.URL
        # aren't list comprehensions fun?!
        This._headers_in = \
          dict((N.lower(), V) for N, V in [Item.split(': ',1) for Item in Env.ALL_RAW.split('\r\n') if Item])

    def get_full_path(This):
        Qs = Env.QUERY_STRING
        return '%s%s' % (This.path, Qs and ('?' + Qs) or '')

    def _load_post_and_files(This):
        "Populates This._post and This._files"
        Ctype = Env.HTTP_CONTENT_TYPE or ''
        if Ctype.startswith('multipart'):
          This._post, This._files = http.parse_file_upload(This._headers_in, This.raw_post_data)
        else:
          This._post, This._files = http.QueryDict(This.raw_post_data), datastructures.MultiValueDict()
        
    def _get_request(This):
        if not hasattr(This, '_request'):
           This._request = datastructures.MergeDict(This.POST, This.GET)
        return This._request

    def _get_get(This):
        if not hasattr(This, '_get'):
            This._get = http.QueryDict(Env.QUERY_STRING)
        return This._get

    def _set_get(This, get):
        This._get = get

    def _get_post(This):
        if not hasattr(This, '_post'):
            This._load_post_and_files()
        return This._post

    def _set_post(This, post):
        This._post = post

    def _get_cookies(This):
        if not hasattr(This, '_cookies'):
            This._cookies = http.parse_cookie(This._headers_in.get('cookie', ''))
        return This._cookies

    def _set_cookies(This, cookies):
        This._cookies = cookies

    def _get_files(This):
        if not hasattr(This, '_files'):
            This._load_post_and_files()
        return This._files

    def _get_meta(This):
        "Lazy loader that returns This.META dictionary"
        if not hasattr(This, '_meta'):
            This._meta = {
                'AUTH_TYPE':         Env.AUTH_TYPE,
                'CONTENT_LENGTH':    Env.CONTENT_LENGTH,
                'CONTENT_TYPE':      Env.CONTENT_TYPE,
                'GATEWAY_INTERFACE': Env.GATEWAY_INTERFACE,
                'PATH_INFO':         Env.PATH_INFO,
                'PATH_TRANSLATED':   Env.PATH_TRANSLATED,
                'QUERY_STRING':      Env.QUERY_STRING,
                'REMOTE_ADDR':       Env.REMOTE_ADDR,
                'REMOTE_HOST':       Env.REMOTE_HOST,
                'REMOTE_IDENT':      None,
                'REMOTE_USER':       Env.REMOTE_USER,
                'REQUEST_METHOD':    Env.REQUEST_METHOD,
                'SCRIPT_NAME':       Env.SCRIPT_NAME,
                'SERVER_NAME':       Env.SERVER_NAME,
                'SERVER_PORT':       Env.SERVER_PORT,
                'SERVER_PROTOCOL':   Env.SERVER_PROTOCOL,
                'SERVER_SOFTWARE':   Env.SERVER_SOFTWARE
            }
            for key, value in This._headers_in.items():
                key = 'HTTP_' + key.upper().replace('-', '_')
                This._meta[key] = value
        return This._meta

    def _get_raw_post_data(This):
        try:
            return This._raw_post_data
        except AttributeError:
            This._raw_post_data = Read()
            return This._raw_post_data

    def _get_user(This):
        if not hasattr(This, '_user'):
            from django.models.auth import users
            try:
                user_id = This.session[users.SESSION_KEY]
                if not user_id:
                    raise ValueError
                This._user = users.get_object(pk=user_id)
            except (AttributeError, KeyError, ValueError, users.UserDoesNotExist):
                from django.parts.auth import anonymoususers
                This._user = anonymoususers.AnonymousUser()
        return This._user

    def _set_user(This, user):
        This._user = user

    GET = property(_get_get, _set_get)
    POST = property(_get_post, _set_post)
    COOKIES = property(_get_cookies, _set_cookies)
    FILES = property(_get_files)
    META = property(_get_meta)
    REQUEST = property(_get_request)
    raw_post_data = property(_get_raw_post_data)
    user = property(_get_user, _set_user)

class PyISAPIeHandler(BaseHandler):
    def __call__(This):
        from django.conf import settings


        if This._request_middleware is None:
            This.load_middleware()

        dispatcher.send(signal=signals.request_started)

        try:
            request = PyISAPIeRequest()
            response = This.get_response(request)

            # Apply response middleware
            for middleware_method in This._response_middleware:
                response = middleware_method(request, response)

        finally:
            dispatcher.send(signal=signals.request_finished)


        process_response(response)

def process_response(http_response):
    from django.conf import settings
    for itm in http_response.items():
      Header("%s: %s" % itm)
    for c in http_response.cookies.values():
      Header('Set-Cookie: ' + c.output(header=''))
    Header(Status = http_response.status_code)
    for chunk in http_response:
        Write(chunk)

