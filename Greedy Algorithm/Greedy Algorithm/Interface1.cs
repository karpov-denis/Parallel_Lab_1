using System;
using System.Collections.Generic;
using System.Text;

namespace Greedy_Algorithm
{
    interface IResultData
    {
        int ContainersNum { get; }
        List<List<int>> ThingsInContainers {get;}
}

interface IInputData
{
    int ThingsNum { get; }
    int ContainersCapacity { get; }
    IThing[] Things { get; }
}

interface IThing
{
       public int id { get; set; }
       public int Mass { get; set;}
}
    interface IPacker
    {
        IResultData Pack(IInputData inputData);
    }
}
