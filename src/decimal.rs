#[derive(Clone, Copy, Debug)]
pub struct Decimal {
    bits: [u32; 4],
}

#[derive(Clone, Copy, Debug)]
pub struct BigDecimal {
    bits: [u32; 8],
}