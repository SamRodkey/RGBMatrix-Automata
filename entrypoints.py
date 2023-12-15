import time

import time

import displayio

from adafruit_itertools import cycle

from helpers import create_display_objects, scroll_up_one_row, apply_rule_to_row_periodic_bc

import two_color_automata_rules as two

def simple_looper(clear_on_rule_change=True):
    """a simple program which loops over multiple two color automata rules in an infinite loop using periodic boundary conditions for edges"""

    # Create a two color palette
    palette = displayio.Palette(2)

    palette[0] = 0x000000  # false color
    palette[1] = 0x000086  # true color

    displayio.release_displays()

    display, bitmap = create_display_objects(palette)

    # set the entire display to zero
    bitmap.fill(0)

    # draw an initial non-zero pixel in center of last row
    bitmap[display.width//2, display.height-1] = 1

    # update display to show pixel
    display.refresh()

    # loop to cycle between different rules
    for rule in cycle((two.rule_30, two.rule_110, two.rule_225)):

        # if we want to clear the screen between rule changes
        if clear_on_rule_change:

            # set the entire display to zero
            bitmap.fill(0)

            # draw an initial non-zero pixel in center of last row
            bitmap[display.width//2, display.height-1] = 1

            # update display to show pixel
            display.refresh()
        
        # run this rule through 640 lines (10 complete screen heights)
        for i in range(640):
            
            # scroll everything up one row
            scroll_up_one_row(display, bitmap)

            # apply our rule to the last row of the display
            apply_rule_to_row_periodic_bc(rule, display,  bitmap)

            # refresh display to show result
            display.refresh()