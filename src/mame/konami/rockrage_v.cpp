// license:BSD-3-Clause
// copyright-holders:Manuel Abadia
#include "emu.h"
#include "rockrage.h"

void rockrage_state::rockrage_palette(palette_device &palette) const
{
	uint8_t const *const color_prom = memregion("proms")->base();

	for (int i = 0; i < 256*3; i++)
	{
		// layer0 uses colors 0x00-0x0f; layer1 uses 0x10-0x1f; sprites use 0x20-0x2f
		uint8_t const colorbase = (i / 256) * 16;
		uint8_t const ctabentry = (color_prom[i] & 0x0f) | colorbase;
		palette.set_pen_indirect(i, ctabentry);
	}
}



/***************************************************************************

  Callback for the K007342

***************************************************************************/

K007342_CALLBACK_MEMBER(rockrage_state::rockrage_tile_callback)
{
	if (layer == 1)
		*code |= ((*color & 0x40) << 2) | ((m_vreg & 0x04) << 7); // doesn't use bank here (Tutankhamen eyes blinking)
	else
		*code |= ((*color & 0x40) << 2) | ((bank & 0x03) << 10) | ((m_vreg & 0x04) << 7) | ((m_vreg & 0x08) << 9);
	*color = layer * 16 + (*color & 0x0f);
}

/***************************************************************************

  Callback for the K007420

***************************************************************************/

K007420_CALLBACK_MEMBER(rockrage_state::rockrage_sprite_callback)
{
	*code |= ((*color & 0x40) << 2) | ((*color & 0x80) << 1) * ((m_vreg & 0x03) << 1);
	*code = (*code << 2) | ((*color & 0x30) >> 4);
	*color = 0 + (*color & 0x0f);
}


void rockrage_state::rockrage_vreg_w(uint8_t data)
{
	/* bits 4-7: unused */
	/* bit 3: bit 4 of bank # (layer 0) */
	/* bit 2: bit 1 of bank # (layer 0) */
	/* bits 0-1: sprite bank select */

	if ((data & 0x0c) != (m_vreg & 0x0c))
		machine().tilemap().mark_all_dirty();

	m_vreg = data;
}

/***************************************************************************

  Screen Refresh

***************************************************************************/

uint32_t rockrage_state::screen_update_rockrage(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	m_k007342->tilemap_update();

	bitmap.fill(0, cliprect);
	m_k007342->tilemap_draw(screen, bitmap, cliprect, 0, TILEMAP_DRAW_OPAQUE, 0);
	// Tutankhamen eyes go below sprites
	m_k007342->tilemap_draw(screen, bitmap, cliprect, 1, 0, 0);
	m_k007420->sprites_draw(bitmap, cliprect, m_gfxdecode->gfx(1));
	m_k007342->tilemap_draw(screen, bitmap, cliprect, 0, 1, 0);
	m_k007342->tilemap_draw(screen, bitmap, cliprect, 1, 1, 0);
	return 0;
}
