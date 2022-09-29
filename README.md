# DisplayPort OSD Framebuffer

This takes Betaflight MSP DisplayPort (so-called "canvas" although this is a misnomer as there is another Betaflight "canvas" mode for Pixel OSDs) messages through UDP and renders them to a framebuffer overlaid under the DJI `dji_glasses` menu system.

A custom `font.bin` package may be placed on the root of the goggles SD card, at which point it will override the font in `/blackbox/font.bin`.

SFML (PC/Mac development) and DJI Goggles viewports are available, as well as a *mux* for the Air Unit / Vista, which creates a *pty* and provides filtered MSP access, and reroutes DisplayPort messages to UDP.

# Setup and Installation

## Easy Installation

* Install WTFOS from https://fpv.wtf. WTFOS must be installed on both the goggles and each AU/Vista.
* Install the msp-osd package on each device using WTFOS.
* Reboot.

## Flight Controller Setup

* Ensure that the correct UART is set to use MSP
* Enable MSP DisplayPort

### Betaflight

On *Betaflight*, this is done using the following commands:

```
set osd_displayport_device = MSP
set displayport_msp_serial = <ConfiguratorUART - 1>
set vcd_video_system = PAL
```

Eg.: If the Betaflight Configurator says your DJI VTx is attached to UART2, the value for **<ConfiguratorUART - 1>** is **1** - so you would use ```set displayport_msp_serial = 1```.

For betaflight - ensure you set the Video Format to PAL or Auto in the OSD tab - otherwise, you don't get access to the whole OSD area. Note that currently, BF Configurator hides these options once you switch to MSP for OSD; the last command above should have done this for you.

#### Fake HD

Betaflight's OSD supports a 30 * 16 Grid, which looks large/blocky when displayed in the DJI Goggles.

Until Betaflight adds support for a larger grid, as a workaround, we have a mode called "FakeHD". It chops up the Betaflight OSD into sections and positions them evenly around an HD grid (with gaps between). Two rows are left unsplit + centered to use for warnings, and the bottom row is unsplit/offset to sit between the DJI OSD elements. This then allows the use of smaller fonts - so it looks nicer/more in keeping with the built in Goggles OSD (but you still only have 30 columns / 16 rows to configure.... and you have gaps to contend with).

A diagram to help...

| Before (in Configurator) | After (in Goggles) |
| -------|-------|
|![FakeHD Before](/docs/img/fakehd_before.png "Before")|![FakeHD After](/docs/img/fakehd_after.png "After")|

##### To configure/enable:

1. Plug in your Goggles + connect them to the WTFOS Configurator.
2. Click on the `CLI` tab.
3. Type/paste `package-config set msp-osd fakehd_enable true` and press Enter.
4. Type/paste `package-config apply msp-osd` and press Enter.
5. Optionally, place custom fonts in the root of your sd card, using the names `font_bf_hd.bin` / `font_bf_hd_2.bin` (NB: FakeHD no longer uses font_hd.bin / font_hd_2.bin)

Configuration of the grid is also possible; see below.

No air unit/vista config is required.

##### Menu Switching - Getting rid of gaps when displaying Menu / Post Flight Stats + displaying centered:

In order to have menus (accessible in Betaflight using stick commands) and post-flight stats appear in the center of the screen while using FakeHD, rather than having gaps + looking broken, you should set up menu switching.

FakeHD can use the presence/absence of a character in the OSD as a switch to indicate when you are in reular OSD mode or in the menu/stats and switch to centering temporarily when needed.

By default, the `Throttle Position` icon is used (character 4) - but you can set any character you want. It needs to be something that doesn't flash or changge in the regular OSD, and ideally (but not essential) something that is never in the menu/post flight stats. The icons next to various elements are obvious choices here. You can set this using the `fakehd_menu_switch` configuration parameter.

Betaflight has a list here: https://github.com/betaflight/betaflight/blob/master/docs/osd.md


If you want to use FakeHD with some other Flight Controller, you will need to find an appropriate icon. (Let us know - we can include the information here).

Finally, if you don't have anything in your OSD that works for menu switching, you can hide the menu switching character and the subsequent 5 characters, allowing you to add the `Throttle Position` element but not have to see it on screen. This is enabled by setting `fakehd_hide_menu_switch` to true.

Notes:

 - Because of this switching feature, if you change to a different quad or OSD config (specifically the switch element is in a different place), FakeHD will center - you will need to reboot your Goggles to get it to recognise the switch element in a different location.

 - Also because of this switching, if you are editing OSD in the configurator with the goggles on to preview and you move the switching element around, it will cause the gaps to be disabled and everything to center. The new location of the switching element will be found next time you reboot the goggles and it'll work as normal.

##### I don't want gaps at all...

Set config `fakehd_lock_center` to true and the center locking used for the menu / post flight stats will be enabled permanently (ie: you fly with it as well) - basically it places your 30 x 16 SD grid into the middle of an HD grid, there's never any gaps - see diagram below + compare to diagrams above.

| After/Centered (in Goggles) `fakehd_lock_center` |
|-------|
|<img src="/docs/img/fakehd_centered.png" alt="After / Centered"  height=200 /> |

##### Customising the default FakeHD grid.

By default, FakeHD positions your SD grid into the HD grid as per the before/after diagram above.

If this doesn't work for you for whatever reason, some customisation is available. It's necessarily a little complicated.

Each row can be set to one of:

| Code | Description |
|---|----|
| L | Left aligned, the SD row occupies cell 1-30, no gaps |
| C | Center aligned, the SD row occupies cell 16-45, no gaps |
| R | Right aligned, , the SD row occupies cell 30-59, no gaps |
| W | Split A - Row is split in 3, the FakeHD default, filling cells 1-10, 26-35, 50-59 |
| T | Split B - Row is split in 2, touching the sides - filling cells 1-15 + 45-59 |
| F | Split C - Row is split in 2 and away from the sides - filling cells 11-25 + 35-49 |
| D | DJI Special - Row is centered but pushed a little left; used to posiution the bottom row between the existing DJI OSD elements |

<img src="/docs/img/fakehd_rows.png" alt="Columns"  height=200 />

And then the columns as a whole can be set to one of:

| Code | Description |
|---|----|
| T | Top aligned, OSD occupies rows 1-16  |
| M | Center aligned, OSD occupies cells 4-19, no gaps |
| B | Bottom aligned, , the OSD occupies rows 7-22 |
| S | Split - FakeHD default - split in 3, OSD occupies rows 1 - 5, 9 - 13, 17-22 |

Using the default row config; here's what they all look like:

| T | M | B | S |
| - | - | - | - |
|<img src="/docs/img/fakehd_columns_t.png" alt="T" />|<img src="/docs/img/fakehd_columns_m.png" alt="M" />| <img src="/docs/img/fakehd_columns_b.png" alt="B" />| <img src="/docs/img/fakehd_after.png" alt="S" />|

###### To configure rows

Rows config accepts a 16 character long string; each character configuring it's corresponding row. The default FakeHD config would be set like this:

`package-config set msp-osd fakehd_rows WWWWWWCCWWWWWWWD`

The characters are case sensitive, but the configurator will reject invalid characters.

###### To configure columns

Columns accepts a single character configuring how the whole grid is aligned. The default would be set like this:

`package-config set msp-osd fakehd_columns S`

The characters are case sensitive, but the configurator will reject invalid characters.

### INAV

On *INAV*, this is done by selecting "HDZero VTx" as the Peripheral. Next, select "HD" in the OSD tab. If the iNav OSD appears garbled at first, try entering the iNav menus using the RC sticks, and then exiting the menus. This will force INAV to switch into HD mode a second time.

### Ardupilot

On *Ardupilot*, this is done by setting:

```
SERIALx_PROTOCOL = 42
OSD_TYPE = 5
```

and optionally

`MSP_OPTIONS = 4` to allow the use of a Betaflight font.

More info: https://ardupilot.org/plane/docs/common-msp-osd-overview-4.2.html#dji-goggles-with-wtf-osd-firmware

### KISS Ultra

Select MSP on serial and select DJI WTF as canvas dialect. Thats it.

## Choose a Font

* Download the latest fonts package from https://github.com/bri3d/mcm2img/releases/download/latest/mcm2img-fonts.tar.gz .
* Rename the files for your desired font to `font_<fc variant>` - see table below for examples or take a look at the `fonts` directory for a template for how the file names should look. (If your FC firmware is not listed below, use the generic filenames)
* Place these four files on the root of your Goggles SD card.
* Reboot.

### FC Specific Font File Names

| Flight controller | SD | HD |
| ----------------- | -- | -- |
| Betaflight       | `font_bf.bin`, `font_bf_2.bin` | `font_bf_hd.bin`, `font_bf_hd_2.bin` |
| INAV       | `font_inav.bin`, `font_inav_2.bin` | `font_inav_hd.bin`, `font_inav_hd_2.bin`|
| Ardupilot       | `font_ardu.bin`, `font_ardu_2.bin` | `font_ardu_hd.bin`, `font_ardu_hd_2.bin`|
| KISS Ultra       | `font_ultra.bin`, `font_ultra_2.bin` | `font_ultra_hd.bin`, `font_ultra_hd_2.bin`|
| Generic/Fallback       | `font_ultra.bin`, `font_ultra_2.bin` | `font_ultra_hd.bin`, `font_ultra_hd_2.bin`|

### Suggested Third Party Fonts

Nicer/more modern looking ones:

 - KNIFA / Material - https://github.com/Knifa/material-osd / https://github.com/Knifa/material-osd/releases
 - Shannon Baker - https://drive.google.com/drive/folders/1buxrXqhU46AxE3fwaFDsMb97IiGLVa95

### Generate your own Font (advanced)

* Download [mcm2img](https://github.com/bri3d/mcm2img) and set up a working Python environment to run it.

* Locate the font you'd like to install - it will be a `.mcm` file, in the source code repository or configurator for your Flight Controller.

* For Betaflight: https://github.com/betaflight/betaflight-configurator/tree/master/resources/osd/2
* For INAV: https://github.com/iNavFlight/inav-configurator/blob/master/resources/osd/
* For Ardupilot: https://github.com/ArduPilot/ardupilot/tree/master/libraries/AP_OSD/fonts

* Run `python3 mcm2img.py mcmfile.mcm font RGBA 255 255 255`

* Place the 4 files this makes (font.bin, font_2.bin, font_hd.bin, font_hd_2.bin) on the root of the SD card in the goggles.

* Reboot

You can customize the font color by changing the 255 255 255 RGB values.

## Configuration options

Configuration options can be set using the CLI function in the WTFOS Configurator.

To see available options, type `package-config get msp-osd` and press Enter.

To set an option, type `package-config set msp-osd key value` and press Enter. Options do NOT set immediately, see next step:

To apply options, type `package-config apply msp-osd`.

### Current available options (Goggles):

| Option | Description | Type |
| ------ | ----------- | ---- |
|`fakehd_enable`| enables FakeHD; the other FakeHD options don't do anything if this is disabled. FakeHD is force disabled of the Flight Controller supports proper HD / RealHD | true/false|
|`fakehd_menu_switch`| FakeHD will use this character as the menu switch to detect when you are in menus/postfligght and triggger centering. Defaults to 4 (Betaflight Throttle). | integer/number |
|`fakehd_hide_menu_switch`| FakeHD will hide the menu switch set above; and the next 5 characters | true / false |
| `fakehd_rows` | FakeHD row alignment config, each character configures the alignment for one row | 16 characters, each one of L C R W T F D |
| `fakehd_columns` | FakeHD column alignment config | Single character, one of T M B S |
|`fakehd_lock_center`| Lock FakeHD in centered mode all the time; no gaps/spreading out even when you are flying.| true / false |
|`show_au_data`| enables AU data overlay on the right | true/false|
|`show_waiting`| enables or disables MSP WAITING message | true/false.|


So for example, to disable the WAITING message:

Click the CLI tab.

Type `package-config set msp-osd show_waiting false` and press ENTER.

Next, Type `package-config apply msp-osd` and press ENTER.

### Current available options (Air Unit/Vista):

None

## FAQ / Suggestions

### How do I create a new font (for INAV, Ardupilot, etc.)?

Use [mcm2img](https://github.com/bri3d/mcm2img).

```
python3 mcm2img.py mcmfile.mcm font.bin RGBA
```

### Why is everything so big / can I make the text smaller (betaflight)?

Betaflight does not support HD OSD. So you have the same 30 * 16 grid as analog uses. The field of view in the DJI goggles makes this look big.

We have a workaround that will work for some use cases; see fakehd under Betaflight above.

Otherwise, you can swap to a different font to make the characters smaller, but the grid spacing is the same.

### How can I get my INAV/ArduPilot/Kiss Ultra OSD closer to the edge of the screen / Why is FakeHD closer to the edges?

 - The current MSP HD grid size (ie: hd in INAV/ArduPilot/Kiss Ultra) is 50 characters wide. We'll call this RealHD.
 - The goggles need 60 characters to go edge to edge - so the 50 in the hd grid doesn't quite fill it
 - So the RealHD grid is displayed centered in the goggles - gaps on both edges.
 - FakeHD had no compatibility constraints like this so we were able to use the full width of the screens.
 - Consequently, FakeHD can get nearer the edges.
 - Currently no solution to get RealHD closer to the edges.

### What is RealHD

Sometimes we refer to the proper MSP OSD HD grid supported by ArduPilot / Kiss Ultra / INAV as RealHD, to distinguish from FakeHD.
# Compiling (development and debugging)

To build for DJI, install the [Android NDK](https://developer.android.com/ndk/downloads) and add the NDK toolchain to your PATH, then use `ndk-build` to build the targets.

To build for UNIXes, install CSFML and run:

```
make -f Makefile.unix
```

Provided targets and tools are:

* `msp_displayport_mux` - takes MSP DisplayPort messages, bundles each frame (all DisplayPort messages between Draw commands) into a single UDP Datagram, and then blasts it over UDP. Also creates a PTY that passes through all _other_ MSP messages, for `dji_hdvt_uav` to connect to.
* `libdisplayport_osd_shim.so` - Patches the `dji_glasses` process to listen for these MSP DisplayPort messages over UDP, and blits them to a DJI framebuffer screen using the DJI framebuffer HAL `libduml_hal` access library, and a converted Betaflight font stored in `font.bin`.
* `osd_sfml` - The same thing as `osd_dji`, but for a desktop PC using SFML and `bold.png`.

Additional debugging can be enabled using `-DDEBUG` as a CFLAG.

## Custom Build Installation (Goggles)

There's a slightly different process for V1 vs V2 Goggles, they renamed some bits between the two.

### FPV Goggles V1

```
ndk-build
adb shell setprop dji.dji_glasses_service 0
adb push libs/armeabi-v7a/libdisplayport_osd_shim.so /tmp
adb shell LD_PRELOAD=/tmp/libdisplayport_osd_shim.so dji_glasses_original -g
```

### FPV Goggles V2

```
ndk-build
adb shell setprop dji.glasses_wm150_service 0
adb push libs/armeabi-v7a/libdisplayport_osd_shim.so /tmp
adb shell LD_PRELOAD=/tmp/libdisplayport_osd_shim.so dji_gls_wm150_original -g
```

### Air Unit / Air Unit Lite (Vista)

```
ndk-build
adb push msp_displayport_mux /blackbox
adb shell setprop dji.hdvt_uav_service 0
adb shell mv /dev/ttyS1 /dev/ttyS1_moved
adb shell nohup /blackbox/msp_displayport_mux 192.168.41.2 /dev/ttyS1_moved /dev/ttyS1 &
adb shell setprop dji.hdvt_uav_service 1
```

This tells the displayport mux to send data from /dev/ttyS1_moved to 192.168.41.2 (goggles) and to create a fake serial port at /dev/ttyS1 with the displayport messages filtered out.

Optionally, you can add `-f`, like `nohup /blackbox/msp_displayport_mux -f 192.168.41.2 /dev/ttyS1_moved /dev/ttyS1` to put the serial port in a faster 230400 baud mode, and set the MSP serial port in your flight controller to 230400 to try to improve the framerate.

You can also omit `setprop dji.hdvt_uav_service 1` , which will improve your OSD framerate at the expense of disabling all Air Unit / Vista side coordination functionality (AU recording, channel changes, some RC features, etc.).

Enjoy.

## Additional Reading / Learning

https://github.com/fpv-wtf/margerine/wiki

## Shoutouts / Thank You

* http://github.com/fpv-wtf team, for making this all possible and very helpful random bits of advice
