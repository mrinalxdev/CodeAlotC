package threadinggo

// This piece of code shows an unique way of implementation of threading in golang,
// concurrent transaction processing and thread safety using mutex
// Used Waitgroup and Mutex Locking for concurrent transaction and safety locking

import (
	"fmt"
	"sync"
	"time"
)

type Account struct {
	balance float64
	mutex   sync.Mutex
}

type Transaction struct {
	account *Account
	amount  float64
}

func NewAccount(balance float64) *Account {
	return &Account{balance: balance}
}

func (a *Account) Deposit(amount float64) {
	a.mutex.Lock()
	defer a.mutex.Unlock()
	a.balance += amount
}

func (a *Account) Withdraw(amount float64) error {
	a.mutex.Lock()
	defer a.mutex.Unlock()

	if a.balance < amount {
		return fmt.Errorf("insufficient balance")
	}
	a.balance -= amount
	return nil
}

func TransactionProcessor(transactions []*Transaction, wg *sync.WaitGroup) {
	defer wg.Done()
	for _, transaction := range transactions {
		switch {
		case transaction.amount > 0:
			transaction.account.Deposit(transaction.amount)
		case transaction.amount < 0:
			err := transaction.account.Withdraw(-transaction.amount)
			if err != nil {
				fmt.Println(err)
			}

		}
		time.Sleep(100 * time.Millisecond)
	}
}

func main() {
	accounts := []*Account{
		NewAccount(1000),
		NewAccount(300),
		NewAccount(2000),
	}

	transactions := []*Transaction{
		{accounts[0], 500},
		{accounts[1], -200},
		{accounts[0], -300},
		{accounts[2], 1000},
		{accounts[1], 150},
	}

	var wg sync.WaitGroup
	wg.Add(len(transactions))

	for _, transaction := range transactions {
		go TransactionProcessor([]*Transaction{transaction}, &wg)
	}

	wg.Wait()

	fmt.Println("Final account balances : ")
	for i, account := range accounts {
		fmt.Printf("Account %d: $%.2f\n", i+1, account.balance)
	}
}
