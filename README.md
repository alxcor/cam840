# cam840
Some small code samples used to monitor variables (using a dynamic graph) or to log alarms using Create MyHMI /3GL for SINUMERIK

![header](/docs/images/header.png)

web page:  [alxcor.github.io/cam840](https://alxcor.github.io/cam840).

## Cam840, Create MyHMI /3GL

<b>Create MyHMI /3GL</b> can be used to create HMI extensions for the SINUMERIK 840D sl and SINUMERIK ONE or stand-alone user interfaces for SINUMERIK 840D sl, SINUMERIK ONE, and SINUMERIK MC.

SINUMERIK Create MyHMI /3GL allows high-level language applications to be developed in the C++ programming language in conjunction with Qt for SINUMERIK Operate on SINUMERIK PCU 50.5, SIMATIC IPC427E or IPC477E, and on SINUMERIK NCU 7x0.

Here are some small code samples used to monitor variables (using a dynamic graph) or to log alarms...
web page:  [Create MyHMI/3GL Manual...](https://support.industry.siemens.com/cs/document/109783268/sinumerik-840d-sl-sinumerik-integrate-create-myhmi-3gl-?dti=0&lc=en-US).

## Identify target Sinumerik system

The HMI (Human Machine Interface) on the Sinumerik system may be based on the native Linux system, running from the CF card of the NCU unit, or on a Windows system installed on a distinct PC unit, connected to the NC via network.

In order to install a new app is necessary to identify <b>the operating system</b> (Linux or Windows), <b>the computer</b> running the HMI interface (NCU or an external computer such as PCU [PC Unit] or IPC [Industrial PC]) and <b>the correct install path</b> of the HMI software.

The type of system may be identified on Operating Area 'Setup', 'System data' menu...

![Menu Select](/docs/images/hk_menu.png) 
![Menu Select](/docs/images/sk_setup.png) 
![Menu Select](/docs/images/sk_systemdata.png)

...by checking where the system data are located.

- If a 'System CF card' is present, SINUMERIK Operate runs under <b>Linux</b>   from <b>NCU unit</b>.

- If a 'System HDD'     is present, SINUMERIK Operate runs under <b>Windows</b> from a <b>PCU or IPC unit</b>.

For a system running on Linux, from the CF Card of the NCU Unit, Sinumerik Operate path is:

<b>/card/oem/sinumerik/hmi</b>

For a system running on Windows, from the HDD of the PCU or IPC Unit, Sinumerik Operate path should be identified from Windows settings:

... for a system running Windows XP the path may be:

<b>F:/hmisl/oem/sinumerik/hmi</b>
<b>F:/hmisl/oem/sinumerik/hmi</b>

... while for a system running Windows7 or Windows10 the path may be: <b>C:/Program Files (x86)/Siemens/MotionControl /oem/sinumerik/hmi</b>
## Identify version

The version of the MyHMI/3GL used in application should match the Sinumerik Operate version.

The version of the HMI system (Sinumerik Operate) may be identified on Operating Area 'Diagnosis', 'Version' menu...


