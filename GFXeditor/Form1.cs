using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GfxEditor
{
    public partial class Form1 : Form
    {
        byte[] FW = null;
        private static List<GfxInfo> cbItems = new List<GfxInfo>()
        {
            new GfxInfo(82, 15, 0xD555),
            new GfxInfo(52, 14, 0xD5F9),
            new GfxInfo(52, 14, 0xD661),
            new GfxInfo(28, 10, 0xD6C9),
            new GfxInfo(23, 7, 0xD701),
            new GfxInfo(46, 12, 0xD718),
            new GfxInfo(64, 12, 0xD774),
            new GfxInfo(30, 32, 0xD7F4),
            new GfxInfo(93, 12, 0xD86C),
            new GfxInfo(30, 32, 0xD926),
            new GfxInfo(8, 8, 0xD99E),
            new GfxInfo(8, 8, 0xD9A6),
            new GfxInfo(5, 5, 0xD9AE),
            new GfxInfo(16, 32, 0xD9B3),
            new GfxInfo(16, 32, 0xD9F3),
            new GfxInfo(32, 32, 0xDA33),
            new GfxInfo(76, 12, 0xDAB3)
        };
        public Form1()
        {
            InitializeComponent();
            gfx.DataSource = cbItems;
        }


        private void imageBrowseClicked(object sender, EventArgs e)
        {
            var fd = new OpenFileDialog();
            fd.Filter = "PNG (*.png)|*.png|BMP (*.bmp)|*.vmp";
            fd.InitialDirectory = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            fd.Title = "Please select an image file to be used as replacment.";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                imageTextBox.Text = fd.FileName;
            }
        }

        private void imageChanged(object sender, EventArgs e)
        {
            pngImage.Image = new Bitmap(imageTextBox.Text);
            replace.Enabled = save.Enabled && pngImage.Image != null;
        }

        private void saveClicked(object sender, EventArgs e)
        {
            bool reverse = !BitConverter.IsLittleEndian;
            var bytes = FW.Skip(0x2788).Take(4);
            if (reverse) bytes = bytes.Reverse();


            UInt32 length = BitConverter.ToUInt32(bytes.ToArray(), 0);
            byte[] array = new byte[length];
            Array.Copy(FW, bootloader, array, 0, length);
            UInt16 checksum = CheckSum.crc16xmodem(array);

            byte[] result = BitConverter.GetBytes(checksum);
            Array.Copy(result, 0, FW, length + 0x1800, result.Length);
            result = FW.Skip(bootloader).ToArray();

            string file = Path.GetFileNameWithoutExtension(firmware.Text);
            string edited = firmware.Text.Replace(file, file + "-edit");


            File.WriteAllBytes(edited, result);

        }

        private void replaceClicked(object sender, EventArgs e)
        {
            int width = 0; int height = 0; int offset = 0;

            if (!TryParseAddress(out offset, out width, out height)) return;
            Bitmap bm = new Bitmap(width, height);

            if (pngImage.Image != null) {
                for (int w = 0; w < width; w++) {
                    if (w >= pngImage.Image.Width) continue;
                    for (int h = 0; h < height; h++)
                    {
                        if (h >= pngImage.Image.Height) continue;
                        Color c =(pngImage.Image as Bitmap).GetPixel(w, h);
                        if (c.A < 128) c = Color.White;
                        else {
                            if (c.R >= 128 && c.G >= 128 && c.B >= 128) c = Color.White;
                            else c = Color.Black;
                        }
                        bm.SetPixel(w,h, c);
                    }
                } 
            }
            pngImage.Image = bm;


            List<byte> imageArray = new List<byte>();
            int row = 0;
            int height2 = height;
            int mod = height % 8;
            if (mod != 0) height2 += (8 - mod);

            int rows = height2 / 8;
            while (row < rows)
            {
                for (int x = 0; x < bm.Width; x++)
                {
                    if (x >= bm.Width) continue;
                    byte val = 0;
                    for (int y = 0; y < 8; y++) {
                        int posy = (row *8)+ y;
                        if (posy >= bm.Height) continue;
                        Color c = bm.GetPixel(x, posy);

                        if (c.R < 128 && c.G < 128 && c.B < 128) {
                            val |= (byte)(1 << y);
                        }
                    }
                    imageArray.Add(val);
                }
                row++;
            }

            Array.Copy(imageArray.ToArray(), 0, FW, offset, imageArray.Count);
            showImagefromFW();
            /*
            int row = 0;
            bool brakLoop = false;
            int bytesLen = 0;
            int rows = height / 8;
            while (offset < FW.Length && row < rows)
            {
                for (int j = 0; j < width; j++)
                {
                    bytesLen++;
                    for (int bit = 0; bit < 8; bit++)
                    {
                        bool val = (FW[offset] & (1 << bit)) != 0;
                        bm.SetPixel(j, row * 8 + bit, val ? Color.Black : Color.White);
                    }
                    offset++;
                    if (offset >= FW.Length) { brakLoop = true; break; }
                }


                if (brakLoop) break;
                row++;
            }*/


        }

        private void sizeChanged(object sender, EventArgs e)
        {
            showImagefromFW();
        }

        private void m_address_TextChanged(object sender, EventArgs e)
        {
            showImagefromFW();
        }

        private void gfx_SelectedIndexChanged(object sender, EventArgs e)
        {
            GfxInfo g = gfx.SelectedItem as GfxInfo;
            if (g == null) return;
            m_address.Text = string.Format("{0:X}", g.Address);
            m_width.Text = string.Format("{0}", g.Width);
            m_height.Text = string.Format("{0}", g.Height);
        }


        bool TryParseAddress(out int address, out int width, out int height) {
            address = 0;
            width = 0;
            height = 0;
            string addr = m_address.Text;
            if (addr.StartsWith("0x")) addr = addr.Substring(2);
            if (FW == null) return false;
           
            if (!Int32.TryParse(addr, System.Globalization.NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture, out address)) return false ;
           
            int bits = width * height;
            if (FW.Length < address + bits / 8) return false;

            if (!Int32.TryParse(m_width.Text, out width) || !Int32.TryParse(m_height.Text, out height))
            {
                return false;
            }
            return true;
        }

        private void showImagefromFW() {
            save.Enabled = !string.IsNullOrEmpty(firmware.Text) && System.IO.File.Exists(firmware.Text);
            replace.Enabled = save.Enabled && pngImage.Image != null;
            int width = 0; int height = 0; int offset = 0;

            if (!TryParseAddress(out offset, out width, out height)) return;
            Bitmap bm = new Bitmap(width, height);
            int row = 0;
            bool brakLoop = false;
            int bytesLen = 0;
            int height2 = height;
            int mod = height % 8;
            if (mod != 0) height2 += (8 - mod);

            int rows = height2 / 8;
            while (offset < FW.Length && row < rows)
            {
                for (int j = 0; j < width; j++)
                {
                    bytesLen++;
                    for (int bit = 0; bit < 8; bit++)
                    {
                        bool val = (FW[offset] & (1 << bit)) != 0;
                        if(j < width && (row * 8 + bit) < height)
                        {
                            bm.SetPixel(j, row * 8 + bit, val ? Color.Black : Color.White);
                        }
                        
                    }
                    offset++;
                    if (offset >= FW.Length) { brakLoop = true; break; }
                }


                if (brakLoop) break;
                row++;
            }
            //bytes.Text = bytesLen.ToString();
            pictureBox1.Image = bm;
            bm.Save(System.IO.Path.ChangeExtension(firmware.Text, "png"), System.Drawing.Imaging.ImageFormat.Png);

        }

        private void firmwareBrowse_Click(object sender, EventArgs e)
        {
            var fd = new OpenFileDialog();
            fd.Filter = "Bin (*.bin)|*.bin";
            fd.InitialDirectory = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            fd.Title = "Please select an firmware file to edit.";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                firmware.Text = fd.FileName;
            }
        }
        int bootloader = 0x1800;

        private void firmware_TextChanged(object sender, EventArgs e)
        {
            FW = System.IO.File.ReadAllBytes(firmware.Text);
            if (FW.Length < 0xfff0) {
                byte[] fullFW = new byte[FW.Length + bootloader];
                byte[] bootloaderNull = Enumerable.Repeat<byte>(0, bootloader).ToArray();
                Array.Copy(bootloaderNull, 0, fullFW, 0, bootloaderNull.Length);
                Array.Copy(FW, 0, fullFW, bootloader, FW.Length);
                FW = fullFW;
            }
            showImagefromFW();
        }
    }

    public class GfxInfo {
        public int Address;
        public int Width;
        public int Height;
        public string Name = string.Empty;
        public GfxInfo(int w, int h, int a, string n = null) {
            Address = a;
            Width = w;
            Height = h;
            if (n != null) Name = n;
        }
        public override string ToString()
        {
            if (!string.IsNullOrEmpty(Name)) return Name;
            return string.Format("0x{0:X} {1} {2}", Address, Width, Height);
        }
    }
}
