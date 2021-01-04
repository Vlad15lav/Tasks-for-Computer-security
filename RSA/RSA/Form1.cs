using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace RSA
{
    public partial class Form1 : Form
    {
        long p; // p value for creating key
        long q; // q value for creating key

        long n; // n value, max lenght text
        long nEuler; // Euler value
        long e_num; // public key

        long d; // private key


        public Form1()
        {
            InitializeComponent();
        }

        bool isCorrectly()
        // Check Variable
        {
            bool isNumQ = long.TryParse(textBox1.Text.Trim(), out long numA);
            bool isNumP = long.TryParse(textBox2.Text.Trim(), out long numB);

            if (!(isNumQ && isNumP))
            {
                MessageBox.Show("Entry number!");
                return false;
            }
            return true;
        }

        bool isInt(string txt)
        // Check type int
        {
            bool isNum = long.TryParse(txt.Trim(), out long numA);

            if (!isNum)
            {
                MessageBox.Show("Entry number!");
                return false;
            }
            return true;
        }

        bool isPrimeNumber(long num)
        // Check prime number
        {
            bool prost = true; // flag
            for (long i = 2; i <= num / 2; i++)
            {
                if (num % i == 0)
                {
                    prost = false;
                    break;
                }
            }
            return prost;
        }

        long GCD(long a, long b)
        // GCD calculation
        {
            while (a != b)
            {
                if (a > b)
                    a = a - b;
                else
                    b = b - a;
            }
            return a;
        }

        long EuclideanAlg(long a, long b)
        // Fast step Alg Euclidean
        {
            long A = a, B = b, row = 1;
            // Creating table (support)
            while (A % B != 0) // Row number
            {
                row++;

                long temp_mod = A % B;
                A = B;
                B = temp_mod;
            }

            long[,] matrix = new long[row, 7]; // Table
            A = a;
            B = b;
            row = 0;
            // Find A, B, A mod B, [A/B] - the first stage
            while (true)
            {
                matrix[row, 0] = row + 1; // i-row
                matrix[row, 1] = A;
                matrix[row, 2] = B;

                long temp_mod = A % B;
                matrix[row, 3] = temp_mod;
                matrix[row, 4] = A / B;

                A = B;
                B = temp_mod;
                row++;

                if (temp_mod == 0) break;
            }

            // Find X, Y - the second stage
            matrix[row - 1, 5] = 0;
            matrix[row - 1, 6] = 1;
            for (long i = row - 2; i >= 0; i--)
            {
                matrix[i, 5] = matrix[i + 1, 6];
                matrix[i, 6] = matrix[i + 1, 5] - matrix[i + 1, 6] * matrix[i, 4];
            }
            return matrix[0, 6];
        }

        long Encode(long a, long b)
        // Encode/Decode
        {
            long num = b;
            Queue<long> Bqueue = new Queue<long>(); // Bin
            Queue<long> Mqueue = new Queue<long>(); // Division
            // Dec2Bin
            while (true)
            {
                Bqueue.Enqueue(b);
                Mqueue.Enqueue(b % 2);
                if (b / 2 == 0) break;
                b /= 2;
            }
            long row = Bqueue.Count();

            long[,] matrix = new long[row, 3];

            for (long i = row - 1; i >= 0; i--)
            {
                matrix[i, 0] = Bqueue.Dequeue();
                matrix[i, 1] = Mqueue.Dequeue();
            }
            
            matrix[0, 2] = a;
            long num_before = a;

            for (long i = 1; i < row; i++)
            {
                if (matrix[i, 1] == 1) // IF b = 1, ELSE a * a_i^2 mod n 
                {
                    matrix[i, 2] = (num_before * num_before * a) % n;
                    num_before = (num_before * num_before * a) % n;
                }
                else // IF b = 0, ELSE a_i^2 mod n 
                {
                    matrix[i, 2] = (num_before * num_before) % n;
                    num_before = (num_before * num_before) % n;
                }
            }
            
            return num_before;
        }

        private void button1_Click(object sender, EventArgs e)
        // Get Public/Private Key
        {
            // Check p and q
            if (!isCorrectly()) return;
            long p_copy = Convert.ToInt64(textBox1.Text); // Read p and q
            long q_copy = Convert.ToInt64(textBox2.Text);
            if (!isPrimeNumber(p_copy) || !isPrimeNumber(q_copy))
            {
                MessageBox.Show("Write prime number!");
                return;
            }
            if (p_copy <= 1 || q_copy <=1)
            {
                MessageBox.Show("Write number > 1!");
                return;
            }

            p = p_copy;
            q = q_copy;

            n = p * q; // Max key value
            label5.Text = "n = " + n.ToString();

            // Euler value
            nEuler = (p - 1) * (q - 1);

            // Possible e value (GCD(e, Fi(n)) == 1)
            string TextChoose = "Example: ";
            long e_count = 2500;
            if (e_count > n) e_count = n;
            for (long i = 2; i <= e_count; i++)
                if (GCD(i, nEuler) == 1)
                    TextChoose += i.ToString() + " ";
            
            // Select e value
            string UserAnswer = Interaction.InputBox(TextChoose, "Select number!", "");
            if (UserAnswer == "") return;

            // Public Key
            e_num = Convert.ToInt64(UserAnswer);

            // Private Key
            d = nEuler + EuclideanAlg(nEuler, e_num);

            // Show Keys
            textBox3.Text = "(" + e_num.ToString() + "," + n.ToString() + ")";
            textBox4.Text = "(" + d.ToString() + "," + n.ToString() + ")";
            textBox5.Text = e_num.ToString();
            textBox6.Text = d.ToString();
            textBox7.Text = n.ToString();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            e_num = Convert.ToInt64(textBox5.Text);
            d = Convert.ToInt64(textBox6.Text);
            n = Convert.ToInt64(textBox7.Text);
            textBox3.Text = "(" + e_num.ToString() + "," + n.ToString() + ")";
            textBox4.Text = "(" + d.ToString() + "," + n.ToString() + ")";
        }

        private void button2_Click(object sender, EventArgs e)
        // Encode Text
        {
            if (!isInt(textBox5.Text) || !isInt(textBox6.Text) || !isInt(textBox7.Text)) return;
            string text = richTextBox1.Text;
            richTextBox2.Text = "";

            // Time Encode
            var myStopwatch = new System.Diagnostics.Stopwatch();
            myStopwatch.Start();

            for (int i = 0, len = text.Length; i < len; i++)
                try
                {
                    richTextBox2.Text += Encode((int)text[i], e_num).ToString() + " ";
                }
                catch { }
            richTextBox2.Text = richTextBox2.Text.Remove(richTextBox2.Text.Length - 1);

            myStopwatch.Stop();
            label8.Text = "Time Encode - " + myStopwatch.Elapsed.TotalMilliseconds.ToString() + " ms";
        }

        private void button3_Click(object sender, EventArgs e)
        // Decode Text
        {
            if (!isInt(textBox5.Text) || !isInt(textBox6.Text) || !isInt(textBox7.Text)) return;
            string[] text = richTextBox1.Text.Split(' ');
            richTextBox2.Text = "";

            // Time Decode
            var myStopwatch = new System.Diagnostics.Stopwatch();
            myStopwatch.Start();

            for (int i = 0, len = text.Length; i < len; i++)
                try
                {
                    richTextBox2.Text += ((char)Encode(Convert.ToInt32(text[i]), d)).ToString();
                }
                catch { }

            myStopwatch.Stop();
            label8.Text = "Time Decode - " + myStopwatch.Elapsed.TotalMilliseconds.ToString() + " ms";
        }

    }
}