NetworkProxyFactory
===================

Subclassing QNetworkProxyFactory to support some common proxy usages.

Originally I wrote this to parse and run PAC scripts.
Then I think it useful to support all common usage one would find on an average browser.
So be it.

Now supporting:
- Direct connection.
- System default proxies.
- Proxy Auto Configuration (PAC) scripts.

Todo:
- Use a single proxy for all connection.
- Use different proxies for different schemes.

Reference:
- [How to handle Proxy PAC configuration with Qt](http://ivandemarino.me/2011/03/21/How-to-handle-Proxy-PAC-configuration-with-Qt/) by Ivan De Marino
