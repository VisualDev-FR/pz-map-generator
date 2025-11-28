use std::{
    collections::HashMap,
    io::{Result, Seek},
};

use crate::{
    buf_reader_extensions::BinReader,
    cell_coord::CellCoord,
    constants::{BLOCK_SIZE_IN_SQUARE, BytesCursor, SQUARE_PER_BLOCK},
    lotheader::Lotheader,
};

pub struct SquareData {}

pub struct Lotpack {
    pub magic: String,
    pub version: i32,
    pub square_datas: HashMap<CellCoord, SquareData>,
}

impl Lotpack {
    pub fn from_bytes(cursor: &mut BytesCursor, r_header: &Lotheader) -> Result<Lotpack> {
        cursor.seek(std::io::SeekFrom::Start(0))?;

        let lotpack = Lotpack {
            magic: cursor.read_n_chars(4)?,
            version: cursor.read_i32()?,
            square_datas: Lotpack::read_square_datas(cursor, r_header)?,
        };

        Ok(lotpack)
    }

    fn read_square_datas(
        cursor: &mut BytesCursor,
        header: &Lotheader,
    ) -> Result<HashMap<CellCoord, SquareData>> {
        let blocks_count = cursor.read_i32()?;
        let table_offset = cursor.position();
        let mut square_datas = HashMap::new();

        for block_index in 0..blocks_count {
            // set cursor in the next table index
            cursor.set_position(table_offset + (block_index * 8) as u64);

            // set the cursor to the next square data sequence position
            let position = cursor.read_i32()? as u64;
            cursor.set_position(position);

            Lotpack::read_square_data(cursor, &mut square_datas, header, block_index as i32)?;
        }

        Ok(square_datas)
    }

    fn read_square_data(
        cursor: &mut BytesCursor,
        hashmap: &mut HashMap<CellCoord, SquareData>,
        header: &Lotheader,
        block_index: i32,
    ) -> Result<()> {
        let mut skip: i32 = 0;
        for z in 0..(header.max_layer - header.min_layer) {
            if skip >= SQUARE_PER_BLOCK as i32 {
                skip -= SQUARE_PER_BLOCK as i32;
                continue;
            }

            for x in 0..BLOCK_SIZE_IN_SQUARE {
                if skip >= BLOCK_SIZE_IN_SQUARE as i32 {
                    skip -= BLOCK_SIZE_IN_SQUARE as i32;
                    continue;
                }

                for y in 0..BLOCK_SIZE_IN_SQUARE {
                    if skip > 0 {
                        skip -= 1;
                        continue;
                    }

                    let count = cursor.read_i32()?;

                    if count == -1 {
                        skip = cursor.read_i32()?;

                        if skip > 0 {
                            skip = -1;
                            continue;
                        }
                    }
                    if count > 1 {
                        let coord = CellCoord::new(block_index as u16, x as i8, y as i8, z as i8);
                        let square_data = Lotpack::read_square(cursor, count)?;

                        hashmap.insert(coord, square_data);
                    }
                }
            }
        }

        Ok(())
    }

    fn read_square(cursor: &mut BytesCursor, count: i32) -> Result<SquareData> {

        let _value = count;
        cursor.seek(std::io::SeekFrom::Start(0))?;
        unimplemented!()
    }
}

// block_index: 0 -> 1024 (u16)
// x:   0 -> 8  (i8)
// y:   0 -> 8  (i8)
// z: -32 -> 32 (i8)

//   0   0   0   0   0   0   0   0
// 128  64  32  16   8   4   2   1
