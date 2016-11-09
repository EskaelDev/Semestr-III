using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Drugie_spotkanie
{
    class Kot : Ssak
    {
        public Kot (string FirstName, string LastName) : base(FirstName, LastName)
            {
            }
        public override void Name()
        {
            Console.WriteLine(this.FirstName);
        }

    }
}
