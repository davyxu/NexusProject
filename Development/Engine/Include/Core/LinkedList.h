#pragma once

namespace NekoEngine
{
	/**
	 * Encapsulates a link in a single linked list with constant access time.
	 */
	template<class ElementType> class TLinkedList
	{
	public:

		/**
		 * Used to iterate over the elements of a linked list.
		 */
		class TIterator
		{
		public:
			TIterator(TLinkedList* FirstLink)
			:	CurrentLink(FirstLink)
			{}

			/**
			 * Advances the iterator to the next element.
			 */
			void Next()
			{
				Assert(CurrentLink);
				CurrentLink = CurrentLink->NextLink;
			}

			/**
			 * Checks for the end of the list.
			 */
			operator bool() const { return CurrentLink != null; }

			// Accessors.
			ElementType& operator->() const
			{
				Assert(CurrentLink);
				return CurrentLink->Element;
			}
			ElementType& operator*() const
			{
				Assert(CurrentLink);
				return CurrentLink->Element;
			}

		private:
			TLinkedList* CurrentLink;
		};

		// Constructors.
		TLinkedList():
			NextLink(null),
			PrevLink(null)
		{}
		TLinkedList(const ElementType& InElement):
			Element(InElement),
			NextLink(null),
			PrevLink(null)
		{}

		/**
		 * Removes this element from the list in constant time.
		 *
		 * This function is safe to call even if the element is not linked.
		 */
		void Unlink()
		{
			if( NextLink )
			{
				NextLink->PrevLink = PrevLink;
			}
			if( PrevLink )
			{
				*PrevLink = NextLink;
			}
			// Make it safe to call Unlink again.
			NextLink = null;
			PrevLink = null;
		}

		/**
		 * Adds this element to a list, before the given element.
		 *
		 * @param Before	The link to insert this element before.
		 */
		void Link(TLinkedList*& Before)
		{
			if(Before)
			{
				Before->PrevLink = &NextLink;
			}
			NextLink = Before;
			PrevLink = &Before;
			Before = this;
		}

		/**
		 * Returns whether element is currently linked.
		 *
		 * @return TRUE if currently linked, FALSE othwerise
		 */
		bool IsLinked()
		{
			return PrevLink != null;
		}

		// Accessors.
		ElementType& operator->()
		{
			return Element;
		}
		const ElementType& operator->() const
		{
			return Element;
		}
		ElementType& operator*()
		{
			return Element;
		}
		const ElementType& operator*() const
		{
			return Element;
		}
		TLinkedList* Next()
		{
			return NextLink;
		}
	private:
		ElementType Element;
		TLinkedList* NextLink;
		TLinkedList** PrevLink;
	};
}

