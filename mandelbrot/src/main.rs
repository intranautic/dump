mod fractal;

fn color(t: f64) -> [u8; 3] {
  let a = (0.5, 0.5, 0.5);
  let b = (0.5, 0.5, 0.5);
  let c = (1.0, 1.0, 1.0);
  let d = (0.0, 0.10, 0.20);
  let r = b.0 * (6.28318 * (c.0 * t + d.0)).cos() + a.0;
  let g = b.1 * (6.28318 * (c.1 * t + d.1)).cos() + a.1;
  let b = b.2 * (6.28318 * (c.2 * t + d.2)).cos() + a.2;
  [(255.0 * r) as u8, (255.0 * g) as u8, (255.0 * b) as u8]
}

fn main() -> Result<(), std::io::Error> {
  let image_width = 1920;
  let image_height = 1080;

  let mut image_buffer = image::ImageBuffer::new(image_width, image_height);

  for (x, y, pixel) in image_buffer.enumerate_pixels_mut() {
    let u = x as f64 / image_height as f64;
    let v = y as f64 / image_height as f64;
    let t = fractal::mandelbrot(
      fractal::Complex {
        a: 2.5 * (u - 0.5) - 1.4,
        b: 2.5 * (v - 0.5)
      }
    );
    *pixel = image::Rgb(color((2.0 * t + 0.5) % 1.0));
  }
  image_buffer.save("mandelbrot.png").unwrap();
  Ok(())
}
