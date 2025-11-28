#[derive(Clone, Copy, PartialEq, Eq, Hash)]
pub struct CellCoord(u32);

impl CellCoord {
    pub fn new(chunk_idx: u16, x: i8, y: i8, z: i8) -> Self {
        let z_packed = (z as i32 + 32) as u32 & 0x3F;
        let y_packed = y as u32 & 0x0F;
        let x_packed = x as u32 & 0x0F;

        let chunk_idx_packed = chunk_idx as u32 & 0x3FFFF;
        let packed_coord = (chunk_idx_packed << 14) | (x_packed << 10) | (y_packed << 6) | z_packed;

        CellCoord(packed_coord)
    }

    pub fn chunk_idx(&self) -> u16 {
        ((self.0 >> 14) & 0x3FFFF) as u16
    }

    pub fn x(&self) -> i8 {
        ((self.0 >> 10) & 0x0F) as i8
    }

    pub fn y(&self) -> i8 {
        ((self.0 >> 6) & 0x0F) as i8
    }

    pub fn z(&self) -> i8 {
        ((self.0 & 0x3F) as i8) - 32
    }

    /// Déballe toutes les coordonnées.
    pub fn unpack(&self) -> (u16, i8, i8, i8) {
        (self.chunk_idx(), self.x(), self.y(), self.z())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_cellcoord_bounds() {
        let coords = [
            (0, 0, 0, -32),    // z min
            (1023, 7, 7, 31),  // z max and x/y max, chunk_idx max
            (512, 2, 2, -5),   // neutral value
            (0, 0, 0, 31),     // z max, min x/y/chunk
            (1023, 0, 0, -32), // z min, max chunk_idx
            (0, 7, 7, 0),      // x/y max, z neutre
            (1023, 7, 7, -32), // combo max/min
        ];

        for &(chunk, x, y, z) in &coords {
            let coord = CellCoord::new(chunk, x, y, z);

            assert_eq!(coord.chunk_idx(), chunk);
            assert_eq!(coord.x() as i8, x);
            assert_eq!(coord.y() as i8, y);
            assert_eq!(coord.z() as i8, z);

            let (c_chunk, c_x, c_y, c_z) = coord.unpack();
            assert_eq!(chunk, c_chunk);
            assert_eq!(x, c_x);
            assert_eq!(y, c_y);
            assert_eq!(z, c_z);
        }
    }
}
