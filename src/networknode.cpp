#include "networknode.hpp"

Neuron::Neuron(){}

Neuron::Neuron(Node* n, Neuron data)
{
    node = n;
    cout << "init neuron " << node << endl;
}

void Neuron::calc()
{
    double temp = 0.0;
    for (auto it = node->input.begin(); it != node->input.end(); it++)
    {
        // printf("temp %d\n",(int)temp);
        temp += (*it)->data.getr<Synapse>().get();
    }
    node->output[0]->data.getr<Synapse>().set(temp);
}

Synapse::Synapse(){}

Synapse::Synapse(Socket* s, Synapse data)
{
    sock = s;
    weight = data.weight;
    val = data.val;
    cout << "init synapse " << sock << " " << weight << " " << val << endl;
}

void Synapse::set(double value)
{
    val = value;
}

double Synapse::get()
{
    if (sock->link == 0 || sock->link->from == sock)
    {
        return val * weight;
    }
    else
    {
        Socket *other = sock->link->from;
        double rtn = other->data.getr<Synapse>().get();
        return rtn * weight;
    }
}