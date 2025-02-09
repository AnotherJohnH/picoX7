A PREFACE:

These files have been generated in kind of a hurry. The hardware they describe is not designed
as any sort of professional grade equipment, and the project was put together over a couple days.
It is simply meant as a more solid replacement for breadboarded components.

The KiCad project files that originated them aren't fully polished - many aspects need correcting,
such as not-fitted and non-BoM components not all being correctly tagged.

As such, the BoM and placements file have been hand-tuned to match what JLC would expect for hopefully-successful
assembly. JLC's library not having a standardized part origin and rotation also doesn't help.

Some models on there lack previews, making verification impossible.

DETAILS:

Files are provided for "a" and "b" versions of the design. Electrically, they're the same, but B skips the I2C LCD connector.
B was made due to A exceeding 100mm in length, thus increasing the cost of an order by over USD20. B is considered the default.

These haven't been tested yet.

For board area, cost and simplicity savings, a few features have been ommitted - most notably, the headphone output. Only a
line-level output straight from the DAC is supplied.


INSTRUCTIONS:
JLC won't, as of Feb. 6h 2025, place the RP2350 as a standard component, apparently due to its still-recent introduction.
As such, it is necessary to contact support to place an order featuring it, like this project.

Due to an absent model, the inductor for the RP2350, the RP2350 proper, and the MIDI optocoupler haven't had their positions
verified on the PCB. You *MUST* pay for the position verification option (and reply to JLC ok'ing their verification - check
your email). Whenever those models become available the files will probably be updated to reflect that and obviate the need
for this additional fee.

1- Plug the gerbers for the desired version onto JLC's website. Change the color of the PCB if you so desire.
2- Select the assembly toggle switch.
3- Select the desired quantity. JLC will always make 5 copies of the PCB as a minimum, but you can choose to only have two
assembled to save money. That's about a $20 difference between 2 and 5 for this project.
3- Select "Yes" on "Confirm Parts Placement"
4- Leave everything else on the default settings - they should work fine and we're not doing rocket surgery here.
5- Hit next.
6- And again.
7- Add the BoM and Positions files.
8- Ignore the error about certain parts not being in the BoM file by pressing "Continue".
9- Deselect any parts you might wish to not fit - for instance, the MIDI jack and optocoupler, or the MicroSD slot, or the
LCD itself if you already have one at home that you can reuse. Each of the "Extended" parts deselected is at minimum
a $2.50 saving - some, way more than that.
10- Ignore any further spurious error messages.
11- Check that everything looks good and move ahead.
12- See if the cost makes sense - right now it costs about US$93 to fully build 5 boards of model B.
13- Select the product category, add to your cart, add shipping and pay.
14- Wait a week or two.


