#define test
using System;
using System.Collections.Generic;

namespace Greedy_Algorithm
{ 

    public class Thing:IThing
    {
        public int id;
        public int Mass;

        int IThing.id
        {
            get => id;
            set => id = value;
        }
        int IThing.Mass
        {
            get => Mass;
            set => Mass = value;
        }
        public Thing(int _id, int _mass)
        {
            this.id = _id;
            this.Mass = _mass;
        }
    }

    public class Container
    {
        public string Contains;
        public int tekMass;
        public int id;
        public Container(int id)
        {
            this.Contains = "";
            this.tekMass = 0;
            this.id = id;
        }
        public Container(int id, string C, int tekMass)
        {
            this.Contains = C;
            this.tekMass = tekMass;
            this.id = id;
        }
    }
   

    class Program
    {
        public static int Comparator(Thing A, Thing B)
        {
            if (A.Mass > B.Mass)
                return -1;
            return 1;
        }

        public static bool IsPlacable(int MaxMass, int tekMass, Thing A)
        {
            return tekMass + A.Mass <= MaxMass;
        }

        static void Main(string[] args)
        {
            List<Thing> AllElems = new List<Thing>();
            int Amount_of_elems;
            int OneContainer;


            Amount_of_elems =Convert.ToInt32( Console.ReadLine());
            Amount_of_elems++;
            OneContainer = Convert.ToInt32(Console.ReadLine());
            string all = Console.ReadLine();
            int index = 1;
            foreach (string a in all.Split(" "))
            {
                AllElems.Add(new Thing(index++, Convert.ToInt32(a)));
                if (index == Amount_of_elems)
                    break;
            }
            AllElems.Sort(Comparator);
#if test
            foreach (Thing A in AllElems)
            {
                Console.Write(A.Mass + " ");
            }
            Console.WriteLine();
#endif 

            List<Container> containers = new List<Container>();
            index = 1;
            containers.Add(new Container(index++));
            foreach (Thing A in AllElems)
            {
                bool placed = false;
                foreach(Container cont in containers)
                {
                    if(IsPlacable(OneContainer,cont.tekMass,A))
                    {
                        placed = true;
                        cont.Contains =cont.Contains+A.id+", ";
                        cont.tekMass += A.Mass;
                        break;
                    }
                }
                if(!placed)
                {
                    containers.Add(new Container(index++,Convert.ToString(A.id)+", ",A.Mass));
                    
                }
            }
            Console.WriteLine(containers.Count);
            foreach(Container cont in containers)
            {
                Console.WriteLine(cont.id + "->"+cont.Contains);
            }
        }
        
    }
}
