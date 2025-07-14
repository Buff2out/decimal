#[derive(Clone, Copy, Debug)]
pub struct Dec {
    pub bits: [u32; 4],
}

impl Dec {
    pub const BEGIN: usize = 2;
    pub const METAINFO: usize = 3;
}

#[derive(Clone, Copy, Debug)]
pub struct BigDec {
    pub bits: [u32; 8],
}

impl BigDec {
    pub const BEGIN: usize = 6;
    pub const METAINFO: usize = 7;
}

// Проверяем, является ли число Decimal нулём
pub fn is_decimal_zero(num: &Dec) -> bool {
    num.bits[0] == 0 && num.bits[1] == 0 && num.bits[2] == 0
}

// Проверяем, является ли число BigDecimal нулём
pub fn is_big_decimal_zero(num: &BigDec) -> bool {
    num.bits[0] == 0 &&
    num.bits[1] == 0 &&
    num.bits[2] == 0 &&
    num.bits[3] == 0 &&
    num.bits[4] == 0 &&
    num.bits[5] == 0 &&
    num.bits[6] == 0
}

// Получаем знак числа Decimal (true — отрицательное, false — положительное)
pub fn get_decimal_sign(num: &Dec) -> bool {
    ((1_u32 << 31) & num.bits[Dec::METAINFO]) != 0
}

// Получаем знак числа BigDecimal (true — отрицательное, false — положительное)
pub fn get_big_decimal_sign(num: &BigDec) -> bool {
    ((1_u32 << 31) & num.bits[BigDec::METAINFO]) != 0
}