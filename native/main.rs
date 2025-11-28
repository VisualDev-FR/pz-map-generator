mod buf_reader_extensions;
mod buf_writer_extensions;
mod cell_coord;
mod constants;
mod lotheader;
mod lotpack;

use std::{
    fs,
    io::{Cursor, Result},
    time::Instant,
};

use crate::{lotheader::Lotheader, lotpack::Lotpack};

fn main() -> Result<()> {
    const B42: &str = "C:/SteamLibrary/steamapps/common/ProjectZomboidB42/media/maps/Muldraugh, KY";

    let start = Instant::now();
    let mut filescount = 0;

    for entry in fs::read_dir(B42)? {
        let path_buf = entry?.path();
        let path_str = path_buf.to_str().unwrap();

        if path_buf.is_file() && path_buf.extension().and_then(|s| s.to_str()) == Some("lotheader")
        {
            // println!("{}", path_buf.file_name().unwrap().to_str().unwrap());

            let header_content = fs::read(path_str)?;
            let header_hash = md5::compute(&header_content);
            let mut header_cursor = Cursor::new(header_content);
            let header = Lotheader::from_bytes(&mut header_cursor)?;
            let header_bytes = header.to_bytes()?;


            let pack_content = fs::read(path)
            Lotpack::from_bytes(&mut header_cursor, &header)?;

            assert_eq!(header_hash, md5::compute(&header_bytes));

            filescount += 1;
        }
    }

    let duration = start.elapsed();

    println!(
        "{} files parsed in {}.{:.3}s ({:.3}ms / file)",
        filescount,
        duration.as_secs(),
        duration.subsec_millis(),
        duration.as_millis() as f64 / (filescount as f64)
    );

    Ok(())
}
