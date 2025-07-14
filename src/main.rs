pub mod decimal;
pub mod utils;

use decimal::{Dec, BigDec};

fn main() {
    let mut dec = Dec { bits: [0, 0, 0, 0] };
    dec.set_scale(5);
    dec.set_sign(1);
    dec.set_bit(0, true);

    println!("scale: {}", dec.get_scale()); // 5
    println!("sign: {}", dec.get_sign());   // true
    println!("bit 0: {}", dec.get_bit(0));  // true

    let mut big = BigDec { bits: [0; 8] };
    big.set_scale(7);
    big.set_sign(1);
    big.set_bit(0, true);

    println!("big scale: {}", big.get_scale()); // 7
    println!("big sign: {}", big.get_sign());   // true
    println!("big bit 0: {}", big.get_bit(0));  // true
}
