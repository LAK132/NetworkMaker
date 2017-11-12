#include "networknode.hpp"

Neuron::Neuron(){}

Neuron::Neuron(Node* n) {
    node = n;
}

void Neuron::load(JSON& neuron_j) {
    
}

void Neuron::save(JSON& neuron_j) {
    
}

void Neuron::calc() {
    double temp = 0.0;
    for (auto it = node->input.begin(); it != node->input.end(); it++)
    {
        Synapse* p = (Synapse*)&((*it)->data);
        temp += p->get();
    }
    Synapse* p = (Synapse*)&(node->output[0]->data);
    p->set(temp);
}

Synapse::Synapse(){}

Synapse::Synapse(Socket* s) {
    sock = s;
}

void Synapse::load(JSON& synapse_j) {
    weight = synapse_j("weight").get<double>();
    val = synapse_j("val").get<double>();   
}

void Synapse::save(JSON& synapse_j) {
    synapse_j("weight").set(weight);
    synapse_j("val").set(val);
}

void Synapse::set(double value) {
    val = value;
}

double Synapse::get() {
    if (sock->link == 0 || sock->link->from == sock)
    {
        return val * weight;
    }
    else
    {
        Synapse* other = (Synapse*)&(sock->link->from->data);
        double rtn = other->get();
        return rtn * weight;
    }
}