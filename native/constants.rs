use std::io::Cursor;

pub const MIN_LAYER: i32 = -32;
pub const MAX_LAYER: i32 = 32;

pub const BLOCK_SIZE_IN_SQUARE: usize = 8;
pub const SQUARE_PER_BLOCK: usize = BLOCK_SIZE_IN_SQUARE * BLOCK_SIZE_IN_SQUARE;

pub const CELL_SIZE_IN_BLOCKS: usize = 32;
pub const BLOCKS_PER_CELL: usize = CELL_SIZE_IN_BLOCKS * CELL_SIZE_IN_BLOCKS;

pub const LINE_END: [u8; 1] = [0xA; 1];

pub static LOTHEADER_EXT: &str = "lotheader";
pub static LOTPACK_EXT: &str = "lotpack";

pub type BytesBuf = Vec<u8>;
pub type BytesCursor = Cursor<BytesBuf>;
