# Platform based Virtualization and Container

## Background and Introduction

### Virtualization

#### Categories of Virtualization

* VVM (Virtual Machine Monitor) aka. Hypervisor

    A abstract layer between VM and Hardware

* Full-Virtualization: The Guest OS run on it can't feel the "virtual" things exist
  * Privilege instruction
  * User instruction
* Paravirtualization 半虛擬化
  * Need to modify the Guest OS's kernel
  * e.g. Domain (VM in Xen) using "hypercall" (i.e. kind of syscall) to request the privilege instruction to *hypervisor*
* Hardware assisted virtualization

#### Example of Virtual Machine

* **Xen**
  * Para Virtualization
    > [GeeksforGeeks - Virtualization | Xen: Paravirtualization](https://www.geeksforgeeks.org/virtualization-xen-paravirtualization/)
  * Full Virtualization
* **KVM** (Kernel-based Virtual Machine)
  * Only Virtualization on CPU and Memory
* **QEMU**: Full virtualization by instruction translate
* **QEMU-KVM**: Combination of QEMU and KVM
  * Using the [IOCTL](https://en.wikipedia.org/wiki/Ioctl) command through `/dev/kvm` to control virtual machine
  * QEMU fulfill other virtualization that KVM didn't have

KVM Related Tools

TBD

Other

* Full virtualization
  * VMware: non-open sourced
  * VirtualBox: open source
* Para virtualization
  * Hyper-V

#### Virtualization on different level

#### Development Trend of Virtualization

> Xen ---> KVM

### Container

#### Standarization of Container

OCI (Open Container Initiative): Google, Docker, CoreOS, IBM, Microsoft, Red Hat, etc.

Open Container For mat

* Docker
* rkt

Runtime Spec

#### Ecosystem

* Container Core Technique
  * Container Spec: runtime spec, image format spec
  * Container Runtime: lxc, runC, rkt, warden
  * Container Manager: lxd, Docker engine (deamon, cli), rkt cli
  * Container Define Tool (Image): Docker image, Dockerfile, ACL (App Container Image)
  * Container Image Repository (Registries): Docker registry, Docker Hub, Quay.io, Harbor
  * Container OS: CoreOS, Atomic, Ubuntu Core, Canonical Snappy, VMWare Photon
* Container Platform Technique
  * Container Arrange Engine 編排引擎: Docker Compose+Swarm+Machine, Kubernetes, Mesos+Marathon+chronos
  * Container Manage Platform: Rancher, ContainerShip
  * Container App Support Platform: Flynn, Deis, Fig, Lattice, Cloud Foundry, OpenShift, Dokku
* Container Support Technique
  * Container Network: Docker network, Flannel, Weave, Calico, CNI, Pipework, Open vSwitch
  * Service Discover: etcd, Consul, ZooKeeper
  * Monitor: Docker ps/top/stats, Docker stats API, sysdig, cAdvisor/Heapster, Weave Scope, Prometheus
  * Data Management: Rex-Ray
  * ...

#### Container Arrange Engine

Kubernete

* Not so depend on Docker (more impartial). Also support rkt.

Docker Swarm

Mesosphere Marathon

## Docker

Based on Linux Kernel *cgroup* and *namespace*

LXC --> libcontainer --> runC + containerd

* Image
* Container
* Repository

Docker Registry

* Registry Mirror

> 鏡像 Image ≠ 鏡像 Mirror

### Linux Kernel Support behind Docker

#### Namespace

#### Cgroups

#### UnionFS

### Docker Structure

C/S Structure

Docker Client <--> Docker Daemon

## Kubernetes

Node

Pod: container group

### Kubernetes Structure

* Node
  * Master
    * apiserver
    * controller-manager
    * scheduler: allocate pod to node
  * Worker

kube-proxy

## Resources

* [VMWare - Virtualization Overview](https://www.vmware.com/pdf/virtualization.pdf)

Article

* [In-depth Overview of x86 Server Virtualization Technology](https://dzone.com/articles/depth-overview-x86-server)
* [Understanding JVM Internals](https://www.cubrid.org/blog/understanding-jvm-internals)
* [Virtual SCSI Adapters vs. Para Virtual SCSI (pvSCSI) Adapters vs. VM Direct Path I/O](https://clearwaterthoughts.wordpress.com/tag/paravirtualization/)

Wikipedia

* [x86 virtualization](https://en.wikipedia.org/wiki/X86_virtualization)

Slides

* [Full and Para Virtualization](https://www.unf.edu/~sahuja/cloudcourse/Fullandparavirtualization.pdf)
