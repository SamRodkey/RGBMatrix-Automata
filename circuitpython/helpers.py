import board
import rgbmatrix
import framebufferio
import displayio

def create_display_objects(palette, num_colors=3,):

    # settings for 64x64 adafruit matrix display
    # object used as input to instantiate display object

    matrix = rgbmatrix.RGBMatrix(
        width=64, height=64, bit_depth=4,
        rgb_pins=[
            board.MTX_R1,
            board.MTX_G1,
            board.MTX_B1,
            board.MTX_R2,
            board.MTX_G2,
            board.MTX_B2
        ],
        addr_pins=[
            board.MTX_ADDRA,
            board.MTX_ADDRB,
            board.MTX_ADDRC,
            board.MTX_ADDRD,
            board.MTX_ADDRE
        ],
        clock_pin=board.MTX_CLK,
        latch_pin=board.MTX_LAT,
        output_enable_pin=board.MTX_OE
    )

    # instantiate display object, turn auto_refresh to false so we can
    # update display after we've computed the new row
    display = framebufferio.FramebufferDisplay(matrix, auto_refresh=False)

    #  Create a bitmap with specified number of colors
    bitmap = displayio.Bitmap(display.width, display.height, num_colors)

    # Create a TileGrid using the Bitmap and Palette
    tile_grid = displayio.TileGrid(bitmap, pixel_shader=palette)

    # Create a Group
    group = displayio.Group()

    # Add the TileGrid to the Group
    group.append(tile_grid)

    # Add the Group to the Display
    display.root_group = group

    # rotate 180 degrees
    display.rotation = 180

    return display, bitmap

def scroll_up_one_row(display, bitmap):
    """method for scrolling the entire scene up one row"""
    for row in range(1, display.height):
        for col in range(display.width):
            bitmap[col, row-1] = bitmap[col, row]

def apply_rule_to_row_periodic_bc(rule: function, display, bitmap,):
    """applies a cellular automata rule passed as an argument to an entire row one pixel at a time"""

    # set the pixel offset for the start of the last of the last row
    start_pixel = (display.height - 1) * display.width 

    # compute the pixel offset for the start of the next to last row
    prev_row_start_pixel = start_pixel - display.width
    
    # first pixel, offset is zero
    lindex = start_pixel - 1
    cindex = (prev_row_start_pixel )
    rindex = (prev_row_start_pixel + 1)

    # apply rule using values from previous row assuming periodic bc on left side, store in first pixel
    bitmap[start_pixel] = rule(bitmap[lindex], bitmap[cindex], bitmap[rindex])

    for i in range(1, display.width-1):

        # get indices left, right, and center elements on previous row for ith pixel in current row
        lindex = prev_row_start_pixel + i - 1
        cindex = prev_row_start_pixel + i
        rindex = prev_row_start_pixel + i + 1

        # apply rule using values from previous row, store in pixel with offset i
        bitmap[start_pixel+i] = rule(bitmap[lindex], bitmap[cindex], bitmap[rindex])

    i  = display.width - 1
    lindex = prev_row_start_pixel + i - 1
    cindex = prev_row_start_pixel + i
    rindex = prev_row_start_pixel

    # apply rule using values from previous row assuming periodic bc on left side, store in first pixel
    bitmap[start_pixel+i] = rule(bitmap[lindex], bitmap[cindex], bitmap[rindex])


    return  