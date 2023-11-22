#![allow(unused)]

extern crate image;

#[derive(
  Debug,
  Clone,
  Copy,
  PartialEq,
  PartialOrd,
  Default,
)]
pub struct Complex {
  pub a: f64,
  pub b: f64,
}

impl Complex {
  fn real(self) -> f64 { return self.a; }
  fn imag(self) -> f64 { return self.b; }
  // z=a+bi, conj(z)=a-bi
  fn conj(self) -> Self {
    Complex {
      a: self.a,
      b: -self.b,
    }
  }
  // z=a+bi, magn(z)=sqrt(a^2+b^2), 
  fn magn(self) -> f64 {
    (self.a*self.a+self.b*self.b)
      .sqrt()
  }
  // abs(z)^2=a^2+b^2
  fn abs(self) -> f64 { (self.a*self.a+self.b*self.b) }
}

impl std::ops::Add<Complex> for Complex {
  type Output = Self;

  fn add(self, _rhs: Self) -> Self {
    Complex {
      a: self.a+_rhs.a,
      b: self.b+_rhs.b,
    }
  }
}

impl std::ops::Sub<Complex> for Complex {
  type Output = Self;

  fn sub(self, _rhs: Self) -> Self {
    Complex {
      a: self.a-_rhs.a,
      b: self.b-_rhs.b,
    }
  }
}

impl std::ops::Mul<Complex> for Complex {
  type Output = Self;
  // let z,w be C, zw=(ac-bd) + (ad+bc)i
  fn mul(self, _rhs: Self) -> Self {
    Complex {
      a: self.a*_rhs.a-self.b*_rhs.b,
      b: self.a*_rhs.b+self.b*_rhs.a,
    }
  }
}

impl std::ops::Div<Complex> for Complex {
  type Output = Self;
  // let z,w be C, z/w= [(ac+bd)/(c^2+d^2)] + [(bc-ad)/(c^2+d^2)]i
  fn div(self, _rhs: Self) -> Self {
    Complex {
      a: ((self.a*_rhs.a)+(self.b*_rhs.b) / (_rhs.a*_rhs.a+_rhs.b*_rhs.b)),
      b: ((self.b*_rhs.a)-(self.a*_rhs.b) / (_rhs.a*_rhs.a+_rhs.b*_rhs.b)),
    }
  }
}

pub fn mandelbrot(c: Complex) -> f64 {
  let mut z = Complex::default();
  let mut i = 0;
  let max = 256;
  while i < max && z.abs() < 32.0 {
    z = z*z+c;
    i += 1;
  }
  return (i as f64 - z.abs().log2().log2())/(max as f64);
}

